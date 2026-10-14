'use strict';

/**
 * Wraps the WASI-compiled c2wasm compiler (compiler.wasm).
 *
 * The compiler is built with wasi-sdk and runs via a minimal WASI shim.
 * It reads C source from stdin and writes WAT/binary to stdout — the shim
 * wires those to in-memory buffers.
 *
 * Supports two compiler modes:
 *   - "reference": the shipped compiler.wasm (always available)
 *   - "custom": a compiler built from user-modified source (optional)
 */
var CompilerAPI = (function () {
  var referenceModule = null;
  var assemblerModule = null;
  var customModule = null;
  var currentMode = 'reference';
  var loading = null;

  var defaultBase = (function () {
    if (typeof document !== 'undefined' && document.currentScript && document.currentScript.src) {
      return document.currentScript.src.substring(0, document.currentScript.src.lastIndexOf('/') + 1);
    }
    return '/embedded-c-lab/static/vendor/c2wasm/';
  })();
  var basePath = defaultBase;

  function init(customBase) {
    if (customBase) basePath = customBase;
    if (referenceModule) return Promise.resolve();
    if (loading) return loading;

    loading = Promise.all([
      fetch(basePath + 'compiler.wasm').then(function (r) {
        if (!r.ok) throw new Error('compiler.wasm not found (HTTP ' + r.status + ') at ' + basePath + 'compiler.wasm');
        return r.arrayBuffer();
      }).then(function (b) { return WebAssembly.compile(b); }),
      fetch(basePath + 'assembler.wasm').then(function (r) {
        if (!r.ok) throw new Error('assembler.wasm not found (HTTP ' + r.status + ') at ' + basePath + 'assembler.wasm');
        return r.arrayBuffer();
      }).then(function (b) { return WebAssembly.compile(b); })
    ]).then(function (mods) {
      referenceModule = mods[0];
      assemblerModule = mods[1];
    });

    loading.catch(function () { loading = null; });
    return loading;
  }

  /** Return the currently active WebAssembly.Module. */
  function activeModule() {
    if (currentMode === 'custom' && customModule) return customModule;
    return referenceModule;
  }

  /**
   * Create a WASI import object that reads from `inputBytes` and captures
   * stdout/stderr into string arrays.
   *
   * When `virtualFS` is provided (a {filename: string} map), path_open/
   * fd_read/fd_close will serve files from it — enabling #include support
   * for in-browser compilation.
   *
   * Delegates to createWasiHarness() from wasi-harness.js.
   */
  function createWasiImports(inputBytes, rawMode, virtualFS, args) {
    return createWasiHarness({
      mode: 'buffer',
      inputBytes: inputBytes,
      rawMode: rawMode,
      virtualFS: virtualFS,
      args: args
    });
  }

  /** Run a compiler module with given source and options. */
  function runCompiler(mod, cSource, rawMode, virtualFS, args) {
    var inputBytes = new TextEncoder().encode(cSource);
    var ctx = createWasiImports(inputBytes, rawMode, virtualFS, args);
    var instance = new WebAssembly.Instance(mod, ctx.imports);
    ctx.setMemory(instance.exports.memory);

    try {
      instance.exports._start();
    } catch (e) {
      if (!ctx.isExit(e)) throw e;
      if (ctx.exitCode !== 0) {
        var errText = ctx.stderr;
        if ((!errText || !errText.trim()) && rawMode && ctx.stdoutBytes && ctx.stdoutBytes.length > 0) {
          errText = new TextDecoder('utf-8', { fatal: false }).decode(ctx.stdoutBytes);
        }
        if ((!errText || !errText.trim()) && ctx.stdout) {
          errText = ctx.stdout;
        }
        if (!errText || !errText.trim()) {
          errText = 'Compiler exited with code ' + ctx.exitCode;
        }
        throw new Error(errText.trim());
      }
    }

    return ctx;
  }

  function normalizeEmbeddedC(src) {
    if (!src) return src;
    var s = src;

    // Strip standard embedded headers that c2wasm doesn't have in virtual FS
    s = s.replace(/#include\s*<(?:stdint|stdbool|assert|stddef)\.h>/gi, '/* embedded header */');

    // Convert binary literals (e.g. 0b1010 or 0b0) to hexadecimal (0x0A)
    s = s.replace(/\b0b([01]+)\b/gi, function(_, bits) {
      return '0x' + parseInt(bits, 2).toString(16).toUpperCase();
    });

    // Strip GCC __attribute__ extensions like __attribute__((packed))
    s = s.replace(/__attribute__\s*\(\([^)]*\)\)/gi, ' ');

    // Strip unsigned/long suffixes from integer literals: 1u, 1U, 0xFFu, 1UL, 1ULL
    s = s.replace(/\b(0x[0-9a-fA-F]+|[0-9]+)[uU][lL]{0,2}\b/g, '$1');

    // Expand comma-separated fields in struct/union: { int a, b; } -> { int a; int b; }
    s = s.replace(/([a-zA-Z0-9_*]+(?:\s+[a-zA-Z0-9_*]+)*)\s+([a-zA-Z0-9_*]+(?:\s*,\s*[a-zA-Z0-9_*]+)+)\s*;/g, function(_, type, ids) {
      return ids.split(',').map(function(id) {
        return type.trim() + ' ' + id.trim() + ';';
      }).join(' ');
    });

    // Multi-token typedef struct { ... } Name; -> struct Name { ... }; typedef struct Name Name;
    s = s.replace(/typedef\s+struct\s*(?:[a-zA-Z0-9_]*)\s*\{([^}]+)\}\s*([a-zA-Z0-9_]+);/g, function(_, body, name) {
      return 'struct ' + name + ' {' + body + '};\ntypedef struct ' + name + ' ' + name + ';';
    });
    s = s.replace(/typedef\s+union\s*(?:[a-zA-Z0-9_]*)\s*\{([^}]+)\}\s*([a-zA-Z0-9_]+);/g, function(_, body, name) {
      return 'union ' + name + ' {' + body + '};\ntypedef union ' + name + ' ' + name + ';';
    });

    // Multi-token pointer casts: (uint8_t*), (unsigned char*) -> (char*); (uint16_t*) -> (short*); (uint32_t*) -> (unsigned*)
    s = s.replace(/\(\s*(?:unsigned\s+char|uint8_t)\s*\*\s*\)/g, '(char*)');
    s = s.replace(/\(\s*(?:unsigned\s+short|uint16_t)\s*\*\s*\)/g, '(short*)');
    s = s.replace(/\(\s*(?:unsigned\s+int|uint32_t)\s*\*\s*\)/g, '(unsigned*)');

    // Multi-token scalar casts: (unsigned char) -> (unsigned), etc.
    s = s.replace(/\(\s*(?:unsigned\s+char|unsigned\s+short|uint8_t|uint16_t|uint32_t)\s*\)/g, '(unsigned)');
    s = s.replace(/\(\s*(?:int8_t|int16_t|int32_t)\s*\)/g, '(int)');

    // Function return types: uint8_t fn( -> unsigned fn(
    s = s.replace(/\b(?:uint8_t|uint16_t|uint32_t)\s+([a-zA-Z0-9_]+)\s*\(/g, 'unsigned $1(');
    s = s.replace(/\b(?:int8_t|int16_t|int32_t)\s+([a-zA-Z0-9_]+)\s*\(/g, 'int $1(');

    // Replace NULL with 0
    s = s.replace(/\bNULL\b/g, '0');

    // Standard integer types in declarations and parameter lists
    s = s.replace(/\buint8_t\b/g, 'unsigned char');
    s = s.replace(/\buint16_t\b/g, 'unsigned short');
    s = s.replace(/\buint32_t\b/g, 'unsigned int');
    s = s.replace(/\buint64_t\b/g, 'unsigned long');
    s = s.replace(/\bint8_t\b/g, 'char');
    s = s.replace(/\bint16_t\b/g, 'short');
    s = s.replace(/\bint32_t\b/g, 'int');
    s = s.replace(/\bint64_t\b/g, 'long');
    s = s.replace(/\bsize_t\b/g, 'unsigned int');

    // Pre-evaluate constant bitshifts (e.g. (1 << 13) -> 0x2000) for register masks
    s = s.replace(/\(\s*1[uU]?\s*<<\s*([0-9]+)\s*\)/g, function(_, shift) {
      return '0x' + (1 << parseInt(shift, 10)).toString(16);
    });
    s = s.replace(/\b1[uU]?\s*<<\s*([0-9]+)\b/g, function(_, shift) {
      return '0x' + (1 << parseInt(shift, 10)).toString(16);
    });

    // Normalize printf format strings (%u -> %d, %04u -> %04d) for c2wasm parser
    s = s.replace(/"([^"\\]|\\.)*"/g, function(str) {
      return str.replace(/%([0-9]*)[uU]/g, '%$1d');
    });

    return s;
  }

  function compile(cSource) {
    return init().then(function () {
      var normalized = normalizeEmbeddedC(cSource);
      var ctx = runCompiler(activeModule(), normalized, false, null);

      if (ctx.stderr && !ctx.stdout) {
        throw new Error(ctx.stderr);
      }

      return ctx.stdout;
    });
  }

  /**
   * Compile C source to WASM binary bytes.
   *
   * In reference mode: reference compiler with -b flag compiles directly.
   * In custom mode: custom compiler (WAT mode) produces WAT text, then
   * assembler.wasm converts WAT → WASM binary.
   *
   * Returns a Promise<Uint8Array>.
   */
  function compileBinary(cSource) {
    return init().then(function () {
      var normalized = normalizeEmbeddedC(cSource);
      if (currentMode === 'custom' && customModule) {
        // Custom compiler produces WAT text (no -b flag)
        var watCtx = runCompiler(customModule, normalized, false, null);
        var wat = watCtx.stdout;
        if (!wat) {
          throw new Error(watCtx.stderr || 'Custom compiler produced no output');
        }
        // Assemble WAT → WASM binary via standalone assembler
        var asmCtx = runCompiler(assemblerModule, wat, true, null);
        var bytes = asmCtx.stdoutBytes;
        if (bytes.length === 0) {
          throw new Error(asmCtx.stderr || 'Assembler produced no output');
        }
        return bytes;
      }

      // Reference compiler produces binary directly via -b flag
      var ctx = runCompiler(referenceModule, normalized, true, null, ['c2wasm', '-b']);
      var bytes = ctx.stdoutBytes;
      if (bytes.length === 0) {
        throw new Error(ctx.stderr || 'Compiler produced no output');
      }
      return bytes;
    });
  }

  /**
   * Build a custom compiler from modified source files.
   * Uses the reference compiler to compile the source, producing a new
   * WASM binary that becomes the "custom" compiler.
   *
   * @param {Object} sourceFiles - Map of filename → source string
   * @returns {Promise<{size: number}>} - Result with compiled binary size
   */
  function buildCompiler(sourceFiles) {
    return init().then(function () {
      // Build custom compiler using reference compiler with -b flag.
      // The custom compiler will output WAT when used — compileBinary()
      // pipes custom WAT output through assembler.wasm for binary execution.
      var entry = sourceFiles['c2wasm.c'];
      if (!entry) throw new Error('c2wasm.c not found in source files');

      // Build virtual FS from all source files (excluding c2wasm.c which is stdin)
      var vfs = {};
      var keys = Object.keys(sourceFiles);
      for (var i = 0; i < keys.length; i++) {
        if (keys[i] !== 'c2wasm.c') {
          vfs[keys[i]] = sourceFiles[keys[i]];
        }
      }

      var ctx = runCompiler(referenceModule, entry, true, vfs, ['c2wasm', '-b']);

      var bytes = ctx.stdoutBytes;
      if (bytes.length === 0) {
        throw new Error(ctx.stderr || 'Compiler build produced no output');
      }

      return WebAssembly.compile(bytes.buffer.slice(bytes.byteOffset, bytes.byteOffset + bytes.byteLength))
        .then(function (mod) {
          customModule = mod;
          currentMode = 'custom';
          return { size: bytes.length };
        });
    });
  }

  /**
   * Compile C source to debug-instrumented WASM binary.
   * Always uses the reference compiler with -D (debug trace) and -b (binary) flags.
   * Returns a Promise<Uint8Array>.
   */
  function compileDebug(cSource) {
    return init().then(function () {
      var normalized = normalizeEmbeddedC(cSource);
      var ctx = runCompiler(referenceModule, normalized, true, null, ['c2wasm', '-D', '-b']);
      var bytes = ctx.stdoutBytes;
      if (bytes.length === 0) {
        throw new Error(ctx.stderr || 'Compiler produced no output');
      }
      return bytes;
    });
  }

  function useReference() { currentMode = 'reference'; }
  function useCustom() {
    if (!customModule) throw new Error('No custom compiler available');
    currentMode = 'custom';
  }
  function getMode() { return currentMode; }
  function hasCustomCompiler() { return customModule !== null; }

  return {
    compile: compile,
    compileBinary: compileBinary,
    compileDebug: compileDebug,
    buildCompiler: buildCompiler,
    useReference: useReference,
    useCustom: useCustom,
    getMode: getMode,
    hasCustomCompiler: hasCustomCompiler,
    normalizeEmbeddedC: normalizeEmbeddedC,
    init: init
  };
})();

