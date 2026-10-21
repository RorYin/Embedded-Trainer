import json
import os
import subprocess
import sys
import time
from pathlib import Path


def run_verification(target_section: str = "", target_problem: str = ""):
    server = subprocess.Popen([sys.executable, "-m", "uvicorn", "embedded_c_lab:app", "--port", "8765"])
    time.sleep(2)

    html = """<!DOCTYPE html>
<html>
<head>
<script src="/embedded-c-lab/static/vendor/c2wasm/wasi-harness.js"></script>
<script src="/embedded-c-lab/static/vendor/c2wasm/compiler-api.js"></script>
</head>
<body>
<div id="status">Running</div>
<pre id="output"></pre>
<script>
async function run() {
  const out = document.getElementById('output');
  const status = document.getElementById('status');
  try {
    await CompilerAPI.init();
    const sectionsRes = await fetch('/embedded-c-lab/api/v1/sections');
    const sectionsData = await sectionsRes.json();

    const targetSec = TARGET_SECTION;
    const targetProb = TARGET_PROBLEM;

    let totalTests = 0;
    let passedTests = 0;
    let failedTests = 0;
    let log = [];

    for (const sec of sectionsData.sections) {
      if (targetSec && sec.id !== targetSec) continue;
      for (const probId of sec.problems) {
        if (targetProb && probId !== targetProb) continue;

        const probRes = await fetch('/embedded-c-lab/api/v1/sections/' + encodeURIComponent(sec.id) + '/problems/' + encodeURIComponent(probId));
        const prob = await probRes.json();

        if (!prob.tester_code) {
          log.push('SKIP: ' + sec.id + '/' + probId + ' (no tester.c yet)');
          continue;
        }

        const solSource = (prob.solution_code || '') + '\\n\\n' + prob.tester_code;
        let wasmBytes;
        try {
          wasmBytes = await CompilerAPI.compileBinary(solSource);
        } catch (compileErr) {
          log.push('COMPILE_ERR in ' + sec.id + '/' + probId + ': ' + (compileErr.message || String(compileErr)));
          failedTests++;
          continue;
        }

        let probPassed = true;
        for (const c of prob.cases) {
          totalTests++;
          const inputBytes = new TextEncoder().encode(c.input || '');
          const ctx = createWasiHarness({
            mode: 'buffer',
            inputBytes: inputBytes,
            args: ['program']
          });
          const mod = new WebAssembly.Module(wasmBytes);
          const inst = new WebAssembly.Instance(mod, ctx.imports);
          ctx.setMemory(inst.exports.memory);
          try {
            inst.exports._start();
          } catch (e) {
            if (!ctx.isExit(e)) {
              probPassed = false;
              failedTests++;
              log.push('RUNTIME_ERR ' + sec.id + '/' + probId + ' Case ' + c.number + ': ' + (e.stack || e.message || String(e)));
              continue;
            }
          }
          const actual = (ctx.stdout || '').replace(/\\r\\n?/g, '\\n').trim();
          const expected = (c.expected || '').replace(/\\r\\n?/g, '\\n').trim();
          if (actual !== expected) {
            probPassed = false;
            failedTests++;
            log.push('FAIL ' + sec.id + '/' + probId + ' Case ' + c.number + ': actual=\"' + actual + '\", expected=\"' + expected + '\"');
          } else {
            passedTests++;
          }
        }
        if (probPassed) {
          log.push('PASS: ' + sec.id + '/' + probId + ' (' + prob.cases.length + ' cases)');
        }
      }
    }
    out.textContent = log.join('\\n');
    status.textContent = 'DONE: passed=' + passedTests + ' failed=' + failedTests + ' total=' + totalTests;
  } catch (err) {
    status.textContent = 'GLOBAL_ERROR';
    out.textContent = err.message || String(err);
  }
}
run();
</script>
</body>
</html>
""".replace("TARGET_SECTION", json.dumps(target_section)).replace("TARGET_PROBLEM", json.dumps(target_problem))

    runner_path = Path("static") / "verify_runner.html"
    runner_path.write_text(html, encoding="utf-8")

    try:
        edge = r"C:\Program Files (x86)\Microsoft\Edge\Application\msedge.exe"
        cmd = [edge, "--headless", "--virtual-time-budget=10000", "--dump-dom", "http://127.0.0.1:8765/embedded-c-lab/static/verify_runner.html"]
        p = subprocess.run(cmd, capture_output=True, text=True, timeout=25)
        dom = p.stdout
        # Extract pre and status
        import re
        m_status = re.search(r'<div id="status">([^<]+)</div>', dom)
        m_out = re.search(r'<pre id="output">([^<]+)</pre>', dom)
        st = m_status.group(1) if m_status else "UNKNOWN"
        ot = m_out.group(1) if m_out else dom
        print(f"Status: {st}")
        print("Output:\n" + ot)
        return "failed=0" in st and "total=" in st
    finally:
        server.terminate()
        if runner_path.exists():
            runner_path.unlink()


if __name__ == "__main__":
    sec = sys.argv[1] if len(sys.argv) > 1 else ""
    prob = sys.argv[2] if len(sys.argv) > 2 else ""
    run_verification(sec, prob)
