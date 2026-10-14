'use strict';

importScripts('/embedded-c-lab/static/vendor/c2wasm/wasi-harness.js');

self.onmessage = function (event) {
  var msg = event.data;
  if (!msg || msg.type !== 'run') return;

  var inputBytes = new TextEncoder().encode(msg.input || '');
  var ctx = createWasiHarness({
    mode: 'buffer',
    inputBytes: inputBytes,
    args: ['program']
  });

  try {
    var module = new WebAssembly.Module(msg.wasmBytes);
    var instance = new WebAssembly.Instance(module, ctx.imports);
    ctx.setMemory(instance.exports.memory);

    instance.exports._start();
    self.postMessage({
      type: 'done',
      id: msg.id,
      stdout: ctx.stdout,
      stderr: ctx.stderr,
      exitCode: ctx.exitCode !== null ? ctx.exitCode : 0
    });
  } catch (e) {
    if (ctx.isExit(e)) {
      self.postMessage({
        type: 'done',
        id: msg.id,
        stdout: ctx.stdout,
        stderr: ctx.stderr,
        exitCode: ctx.exitCode !== null ? ctx.exitCode : 0
      });
    } else {
      self.postMessage({
        type: 'error',
        id: msg.id,
        message: e && e.message ? e.message : String(e),
        stdout: ctx.stdout,
        stderr: ctx.stderr
      });
    }
  }
};
