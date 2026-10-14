"use strict";

const API = "/embedded-c-lab/api/v1";
const editorHost = document.getElementById("editor");
const runButton = document.getElementById("run-button");
const workspace = document.querySelector(".workspace");
let editor;
let currentExercise;
let progress = {};

function setupSplitters() {
  const root = document.documentElement;
  const narrowScreen = () => window.matchMedia("(max-width: 900px)").matches;
  const sidebarSplitter = document.getElementById("sidebar-splitter");
  const resultsSplitter = document.getElementById("results-splitter");
  const descriptionSplitter = document.getElementById("description-splitter");
  const savedSidebarWidth = Number(localStorage.getItem("embedded-c-lab.sidebar-width"));
  const savedDescriptionWidth = Number(localStorage.getItem("embedded-c-lab.description-width"));
  const savedSidebarHeight = Number(localStorage.getItem("embedded-c-lab.sidebar-height"));
  const savedDescriptionHeight = Number(localStorage.getItem("embedded-c-lab.description-height"));
  const savedResultsHeight = Number(localStorage.getItem("embedded-c-lab.results-height"));
  let sidebarWidth = Number.isFinite(savedSidebarWidth) && savedSidebarWidth > 0 ? savedSidebarWidth : 264;
  let descriptionWidth = Number.isFinite(savedDescriptionWidth) && savedDescriptionWidth > 0 ? savedDescriptionWidth : 340;
  let sidebarHeight = Number.isFinite(savedSidebarHeight) && savedSidebarHeight > 0 ? savedSidebarHeight : 180;
  let descriptionHeight = Number.isFinite(savedDescriptionHeight) && savedDescriptionHeight > 0 ? savedDescriptionHeight : 280;
  let resultsHeight = Number.isFinite(savedResultsHeight) && savedResultsHeight > 0 ? savedResultsHeight : 240;

  function setSidebarSize(size, persist = false) {
    if (narrowScreen()) {
      sidebarHeight = Math.max(120, Math.min(size, 320));
      root.style.setProperty("--sidebar-height", `${sidebarHeight}px`);
      sidebarSplitter.setAttribute("aria-valuemin", "120");
      sidebarSplitter.setAttribute("aria-valuemax", "320");
      sidebarSplitter.setAttribute("aria-valuenow", String(Math.round(sidebarHeight)));
      if (persist) localStorage.setItem("embedded-c-lab.sidebar-height", String(Math.round(sidebarHeight)));
    } else {
      const available = workspace.clientWidth - 2 * 5 - 300 - 240;
      const maximum = Math.max(180, Math.min(480, available));
      sidebarWidth = Math.max(180, Math.min(size, maximum));
      root.style.setProperty("--sidebar-width", `${sidebarWidth}px`);
      sidebarSplitter.setAttribute("aria-valuemin", "180");
      sidebarSplitter.setAttribute("aria-valuemax", "480");
      sidebarSplitter.setAttribute("aria-valuenow", String(Math.round(sidebarWidth)));
      if (persist) localStorage.setItem("embedded-c-lab.sidebar-width", String(Math.round(sidebarWidth)));
    }
  }

  function setDescriptionSize(size, persist = false) {
    if (narrowScreen()) {
      descriptionHeight = Math.max(160, Math.min(size, 520));
      root.style.setProperty("--description-height", `${descriptionHeight}px`);
      descriptionSplitter.setAttribute("aria-valuemin", "160");
      descriptionSplitter.setAttribute("aria-valuemax", "520");
      descriptionSplitter.setAttribute("aria-valuenow", String(Math.round(descriptionHeight)));
      if (persist) localStorage.setItem("embedded-c-lab.description-height", String(Math.round(descriptionHeight)));
    } else {
      const available = workspace.clientWidth - sidebarWidth - 2 * 5 - 300 - 240;
      const maximum = Math.max(240, Math.min(640, available));
      descriptionWidth = Math.max(240, Math.min(size, maximum));
      root.style.setProperty("--description-width", `${descriptionWidth}px`);
      descriptionSplitter.setAttribute("aria-valuemin", "240");
      descriptionSplitter.setAttribute("aria-valuemax", "640");
      descriptionSplitter.setAttribute("aria-valuenow", String(Math.round(descriptionWidth)));
      if (persist) localStorage.setItem("embedded-c-lab.description-width", String(Math.round(descriptionWidth)));
    }
  }

  function setResultsHeight(height, persist = false) {
    const maximum = Math.max(140, Math.min(500, workspace.clientHeight - 260));
    resultsHeight = Math.max(140, Math.min(height, maximum));
    root.style.setProperty("--results-height", `${resultsHeight}px`);
    resultsSplitter.setAttribute("aria-valuenow", String(Math.round(resultsHeight)));
    if (persist) localStorage.setItem("embedded-c-lab.results-height", String(Math.round(resultsHeight)));
  }

  function enableSplitter(element, updateSize, getSize, getAxis, getDirection, defaultSize) {
    element.addEventListener("pointerdown", (event) => {
      if (event.button !== 0) return;
      event.preventDefault();
      const axis = getAxis();
      const start = axis === "x" ? event.clientX : event.clientY;
      const initialSize = getSize();
      element.classList.add("dragging");
      element.setPointerCapture(event.pointerId);
      const move = (moveEvent) => {
        const position = axis === "x" ? moveEvent.clientX : moveEvent.clientY;
        updateSize(initialSize + (position - start) * getDirection());
      };
      const finish = () => {
        element.classList.remove("dragging");
        updateSize(getSize(), true);
        element.removeEventListener("pointermove", move);
        element.removeEventListener("pointerup", finish);
        element.removeEventListener("pointercancel", finish);
      };
      element.addEventListener("pointermove", move);
      element.addEventListener("pointerup", finish);
      element.addEventListener("pointercancel", finish);
    });
    element.addEventListener("keydown", (event) => {
      if (event.key === "Home") {
        event.preventDefault();
        updateSize(defaultSize, true);
        return;
      }
      const axis = getAxis();
      const negativeKey = axis === "x" ? "ArrowLeft" : "ArrowUp";
      const positiveKey = axis === "x" ? "ArrowRight" : "ArrowDown";
      if (event.key !== negativeKey && event.key !== positiveKey) return;
      event.preventDefault();
      const step = event.key === positiveKey ? 16 : -16;
      updateSize(getSize() + step * getDirection(), true);
    });
  }

  const sidebarAxis = () => narrowScreen() ? "y" : "x";
  const descriptionAxis = () => narrowScreen() ? "y" : "x";
  setSidebarSize(sidebarAxis() === "x" ? sidebarWidth : sidebarHeight);
  setDescriptionSize(descriptionAxis() === "x" ? descriptionWidth : descriptionHeight);
  setResultsHeight(resultsHeight);
  enableSplitter(sidebarSplitter, setSidebarSize, () => sidebarAxis() === "x" ? sidebarWidth : sidebarHeight, sidebarAxis, () => sidebarAxis() === "x" ? -1 : 1, 264);
  enableSplitter(descriptionSplitter, setDescriptionSize, () => descriptionAxis() === "x" ? descriptionWidth : descriptionHeight, descriptionAxis, () => 1, 340);
  enableSplitter(resultsSplitter, setResultsHeight, () => resultsHeight, () => "y", () => -1, 240);
  document.getElementById("description-toggle").addEventListener("click", (event) => {
    const button = event.currentTarget;
    const collapsed = button.getAttribute("aria-expanded") === "true";
    workspace.classList.toggle("description-collapsed", collapsed);
    button.setAttribute("aria-expanded", String(!collapsed));
    button.setAttribute("aria-label", collapsed ? "Expand problem description" : "Collapse problem description");
    button.title = collapsed ? "Expand problem description" : "Collapse problem description";
    button.querySelector("span").textContent = collapsed ? "→" : "←";
  });
  window.addEventListener("resize", () => {
    setSidebarSize(sidebarAxis() === "x" ? sidebarWidth : sidebarHeight);
    setDescriptionSize(descriptionAxis() === "x" ? descriptionWidth : descriptionHeight);
    setResultsHeight(resultsHeight);
    sidebarSplitter.setAttribute("aria-orientation", sidebarAxis() === "x" ? "vertical" : "horizontal");
    descriptionSplitter.setAttribute("aria-orientation", descriptionAxis() === "x" ? "vertical" : "horizontal");
  });
  sidebarSplitter.setAttribute("aria-orientation", sidebarAxis() === "x" ? "vertical" : "horizontal");
  descriptionSplitter.setAttribute("aria-orientation", descriptionAxis() === "x" ? "vertical" : "horizontal");
}

function showNotice(message) {
  const results = document.getElementById("result-list");
  results.replaceChildren();
  const notice = document.createElement("div");
  notice.className = "notice";
  notice.textContent = formatErrorDetail(message);
  results.append(notice);
}

function normalizeOutput(value) {
  return String(value || "").replace(/\r\n?/g, "\n").trim();
}

function formatCompilerError(rawError, source) {
  const text = String(rawError || "").trim();
  const match = text.match(/(?:[a-zA-Z0-9_\-\.]+:)?(\d+):(\d+):\s*(error:\s*.+)/);
  if (!match) return text;
  const lineNum = parseInt(match[1], 10);
  const colNum = parseInt(match[2], 10);
  const errDesc = match[3];

  const lines = String(source || "").split("\n");
  const sourceLine = lines[lineNum - 1] ?? "";
  const pointer = " ".repeat(Math.max(0, colNum - 1)) + "^";

  const activeFilename = document.getElementById("filename")?.textContent || "solution.c";
  return `${activeFilename}:${lineNum}:${colNum}: ${errDesc}\n    ${sourceLine}\n    ${pointer}`;
}

function parseCompilerError(msg) {
  const match = String(msg || "").match(/(?:[a-zA-Z0-9_\-\.]+:)?(\d+):(\d+):\s*(error:\s*.+)/);
  if (!match) return null;
  return {
    line: parseInt(match[1], 10),
    col: parseInt(match[2], 10),
    message: match[3],
  };
}

function setEditorErrorMarker(errorInfo) {
  if (!editor || !window.monaco || !errorInfo) return;
  const model = editor.getModel();
  if (!model) return;
  const lineLength = typeof model.getLineLength === "function" ? model.getLineLength(errorInfo.line) : errorInfo.col + 5;
  window.monaco.editor.setModelMarkers(model, "c2wasm", [
    {
      startLineNumber: errorInfo.line,
      startColumn: errorInfo.col,
      endLineNumber: errorInfo.line,
      endColumn: Math.max(errorInfo.col + 1, lineLength + 1),
      message: errorInfo.message,
      severity: window.monaco.MarkerSeverity.Error,
    },
  ]);
  if (typeof editor.revealLineInCenter === "function") {
    editor.revealLineInCenter(errorInfo.line);
  }
}

function clearEditorMarkers() {
  if (!editor || !window.monaco) return;
  const model = editor.getModel();
  if (model) window.monaco.editor.setModelMarkers(model, "c2wasm", []);
}

function runCaseWithTimeout(wasmBytes, input, timeoutMs = 2000) {
  return new Promise((resolve) => {
    let worker = null;
    let timer = null;
    let resolved = false;

    function finish(res) {
      if (resolved) return;
      resolved = true;
      if (timer) clearTimeout(timer);
      if (worker) {
        worker.terminate();
        worker = null;
      }
      resolve(res);
    }

    try {
      worker = new Worker("/embedded-c-lab/static/vendor/c2wasm/wasm-worker.js");
      worker.onmessage = function (event) {
        const data = event.data || {};
        if (data.type === "done") {
          finish({
            stdout: data.stdout || "",
            stderr: data.stderr || "",
            exitCode: data.exitCode,
            error: "",
          });
        } else if (data.type === "error") {
          finish({
            stdout: data.stdout || "",
            stderr: data.stderr || "",
            exitCode: 1,
            error: data.message || "Runtime error",
          });
        }
      };
      worker.onerror = function (err) {
        finish({
          stdout: "",
          stderr: "",
          exitCode: 1,
          error: err && err.message ? err.message : "Worker execution error",
        });
      };
      timer = setTimeout(() => {
        finish({
          stdout: "",
          stderr: "",
          exitCode: null,
          error: `Execution exceeded the ${timeoutMs / 1000}-second time limit.`,
        });
      }, timeoutMs);

      worker.postMessage({ type: "run", wasmBytes: wasmBytes, input: input || "" });
    } catch (workerErr) {
      try {
        const inputBytes = new TextEncoder().encode(input || "");
        const ctx = createWasiHarness({
          mode: "buffer",
          inputBytes: inputBytes,
          args: ["program"],
        });
        const mod = new WebAssembly.Module(wasmBytes);
        const inst = new WebAssembly.Instance(mod, ctx.imports);
        ctx.setMemory(inst.exports.memory);
        try {
          inst.exports._start();
        } catch (e) {
          if (!ctx.isExit(e)) throw e;
        }
        resolve({
          stdout: ctx.stdout,
          stderr: ctx.stderr,
          exitCode: ctx.exitCode !== null ? ctx.exitCode : 0,
          error: "",
        });
      } catch (runErr) {
        resolve({
          stdout: "",
          stderr: "",
          exitCode: 1,
          error: runErr && runErr.message ? runErr.message : String(runErr),
        });
      }
    }
  });
}

function formatErrorDetail(detail) {
  if (typeof detail === "string") return detail;
  if (Array.isArray(detail)) {
    return detail.map((item) => {
      if (typeof item === "string") return item;
      if (!item || typeof item !== "object") return String(item);
      const location = Array.isArray(item.loc) ? item.loc.filter((part) => part !== "body").join(" → ") : "";
      const message = typeof item.msg === "string" ? item.msg : JSON.stringify(item);
      return location ? `${location}: ${message}` : message;
    }).join("\n");
  }
  if (detail && typeof detail === "object") {
    if (typeof detail.message === "string") return detail.message;
    return JSON.stringify(detail, null, 2);
  }
  return String(detail ?? "An unexpected error occurred.");
}

function appendInlineMarkdown(parent, text) {
  const tokenPattern = /(`[^`]+`|\*\*[^*]+\*\*|__[^_]+__|\*[^*]+\*|_[^_]+_|\[[^\]]+\]\([^)]+\))/g;
  let position = 0;
  for (const match of text.matchAll(tokenPattern)) {
    if (match.index > position) {
      parent.append(document.createTextNode(text.slice(position, match.index)));
    }
    const token = match[0];
    let node;
    if (token.startsWith("`")) {
      node = document.createElement("code");
      node.textContent = token.slice(1, -1);
    } else if (token.startsWith("**") || token.startsWith("__")) {
      node = document.createElement("strong");
      node.textContent = token.slice(2, -2);
    } else if (token.startsWith("*") || token.startsWith("_")) {
      node = document.createElement("em");
      node.textContent = token.slice(1, -1);
    } else {
      const link = token.match(/^\[([^\]]+)\]\(([^)]+)\)$/);
      let destination;
      try {
        destination = new URL(link[2], window.location.href);
      } catch {
        destination = null;
      }
      if (destination && ["http:", "https:"].includes(destination.protocol)) {
        node = document.createElement("a");
        node.href = destination.href;
        node.target = "_blank";
        node.rel = "noopener noreferrer";
        node.textContent = link[1];
      } else {
        node = document.createTextNode(token);
      }
    }
    parent.append(node);
    position = match.index + token.length;
  }
  if (position < text.length) parent.append(document.createTextNode(text.slice(position)));
}

function renderMarkdown(container, markdown) {
  container.replaceChildren();
  const lines = String(markdown || "").replace(/\r\n?/g, "\n").split("\n");
  let index = 0;
  while (index < lines.length) {
    const line = lines[index];
    if (!line.trim()) {
      index += 1;
      continue;
    }

    const fence = line.match(/^\s*```([a-zA-Z0-9_-]*)\s*$/);
    if (fence) {
      const codeLines = [];
      index += 1;
      while (index < lines.length && !/^\s*```\s*$/.test(lines[index])) {
        codeLines.push(lines[index]);
        index += 1;
      }
      if (index < lines.length) index += 1;
      const pre = document.createElement("pre");
      const code = document.createElement("code");
      if (fence[1]) code.className = `language-${fence[1]}`;
      code.textContent = codeLines.join("\n");
      pre.append(code);
      container.append(pre);
      continue;
    }

    const heading = line.match(/^(#{1,4})\s+(.+?)\s*#*$/);
    if (heading) {
      const element = document.createElement(`h${heading[1].length + 1}`);
      appendInlineMarkdown(element, heading[2]);
      container.append(element);
      index += 1;
      continue;
    }

    const listItem = line.match(/^\s*([-*+]|\d+[.)])\s+(.+)$/);
    if (listItem) {
      const ordered = /^\d/.test(listItem[1]);
      const list = document.createElement(ordered ? "ol" : "ul");
      while (index < lines.length) {
        const currentItem = lines[index].match(/^\s*([-*+]|\d+[.)])\s+(.+)$/);
        if (!currentItem || /^\d/.test(currentItem[1]) !== ordered) break;
        const item = document.createElement("li");
        appendInlineMarkdown(item, currentItem[2]);
        list.append(item);
        index += 1;
      }
      container.append(list);
      continue;
    }

    if (/^\s*([-*_])(?:\s*\1){2,}\s*$/.test(line)) {
      container.append(document.createElement("hr"));
      index += 1;
      continue;
    }

    const paragraph = document.createElement("p");
    const paragraphLines = [];
    while (index < lines.length && lines[index].trim()) {
      if (
        paragraphLines.length > 0
        && (/^\s*```/.test(lines[index])
          || /^#{1,4}\s+/.test(lines[index])
          || /^\s*([-*+]|\d+[.)])\s+/.test(lines[index]))
      ) break;
      paragraphLines.push(lines[index].trim());
      index += 1;
    }
    appendInlineMarkdown(paragraph, paragraphLines.join(" "));
    container.append(paragraph);
  }
}

async function api(path, options) {
  const response = await fetch(`${API}${path}`, options);
  let body;
  try {
    body = await response.json();
  } catch {
    throw new Error(`The server returned an unreadable response (${response.status}).`);
  }
  if (!response.ok) {
    throw new Error(formatErrorDetail(body.detail) || `Request failed (${response.status}).`);
  }
  return body;
}

async function loadMonaco() {
  const localLoader = "/embedded-c-lab/static/vendor/monaco/min/vs/loader.js";
  const cdnLoader = "https://cdn.jsdelivr.net/npm/monaco-editor@0.52.2/min/vs/loader.js";
  let loaderUrl = cdnLoader;
  try {
    const localResponse = await fetch(localLoader, { method: "HEAD" });
    if (localResponse.ok) loaderUrl = localLoader;
  } catch {
    // The bundled copy is optional; use the CDN when it is unavailable.
  }

  await new Promise((resolve, reject) => {
    const script = document.createElement("script");
    script.src = loaderUrl;
    script.onload = resolve;
    script.onerror = () => reject(new Error("Could not load Monaco. Connect to the internet or bundle Monaco locally."));
    document.head.append(script);
  });

  const base = loaderUrl.slice(0, -"/loader.js".length);
  window.require.config({ paths: { vs: base } });
  await new Promise((resolve, reject) => {
    window.require(["vs/editor/editor.main"], resolve, reject);
  });
  window.monaco.editor.defineTheme("embedded-dark", {
    base: "vs-dark",
    inherit: true,
    rules: [
      { token: "keyword", foreground: "C78AC1" },
      { token: "keyword.directive", foreground: "C78AC1" },
      { token: "keyword.directive.include", foreground: "C78AC1" },
      { token: "keyword.return", foreground: "C78AC1" },
      { token: "keyword.control", foreground: "C78AC1" },
      { token: "string", foreground: "D49A7E" },
      { token: "string.include.identifier", foreground: "D49A7E" },
      { token: "identifier.function", foreground: "D6D6A4" },
      { token: "function", foreground: "D6D6A4" },
      { token: "number", foreground: "B7D1A8" },
      { token: "comment", foreground: "8DC779" },
      { token: "identifier", foreground: "9DD9F5" },
      { token: "delimiter", foreground: "D4D4D4" },
    ],
    colors: {
      "editor.background": "#141414",
      "editorGutter.background": "#141414",
      "editorLineNumber.foreground": "#858585",
      "editorLineNumber.activeForeground": "#c6c6c6",
      "editor.lineHighlightBackground": "#cccccc0a",
      "editor.selectionBackground": "#1e3f60",
      "editorIndentGuide.background1": "#333333",
      "editorIndentGuide.activeBackground1": "#595959",
      "editorCursor.foreground": "#aeafad",
      "editor.selectionHighlightBackground": "#8aabcc26",
      "editorWidget.background": "#1b1b1c",
      "editorWidget.border": "#373737",
      "editorSuggestWidget.background": "#1b1b1c",
      "editorSuggestWidget.border": "#373737",
    },
  });
  editor = window.monaco.editor.create(editorHost, {
    value: "/* Select an exercise to load its starter code. */\n",
    language: "c",
    theme: "embedded-dark",
    automaticLayout: true,
    minimap: { enabled: false },
    fontSize: 14,
    letterSpacing: 0.5,
    scrollBeyondLastLine: false,
    tabSize: 4,
  });
}

function makeResultCard(testCase) {
  const card = document.createElement("article");
  card.className = `result-card ${testCase.passed ? "pass" : "fail"}`;
  const details = document.createElement("details");
  details.className = "result-details";
  details.open = !testCase.passed;
  const title = document.createElement("summary");
  title.className = "result-title";
  title.textContent = `Test ${testCase.number}: ${testCase.passed ? "Passed" : "Failed"}`;
  const output = document.createElement("pre");
  output.className = "result-output";
  output.textContent = `Input:\n${testCase.input}\n\nExpected output:\n${testCase.expected}\n\nYour output:\n${testCase.actual}${testCase.error ? `\n\nError:\n${testCase.error}` : ""}`;
  details.append(title, output);
  card.append(details);
  return card;
}

function updateSourceControls() {
  const toggle = document.getElementById("source-toggle");
  if (!currentExercise) {
    if (toggle) toggle.hidden = true;
    return;
  }
  const filenameEl = document.getElementById("filename");
  const hasSolved = typeof currentExercise.solvedCode === "string";

  if (currentExercise.activeSource === "solution") {
    filenameEl.textContent = currentExercise.solutionFilename;
    toggle.hidden = false;
    toggle.textContent = "Load template";
    toggle.title = "Return to the original problem template";
  } else if (currentExercise.activeSource === "solved") {
    filenameEl.textContent = currentExercise.solvedFilename;
    toggle.hidden = false;
    toggle.textContent = "Load template";
    toggle.title = "Return to the original problem template";
  } else {
    // "template"
    filenameEl.textContent = currentExercise.starterFilename;
    toggle.hidden = !hasSolved;
    if (hasSolved) {
      toggle.textContent = "Load my solved code";
      toggle.title = "Load your previously saved solution";
    }
  }
}

function loadExerciseSource(source) {
  if (!currentExercise) return;
  currentExercise.activeSource = source;
  if (source === "solution") {
    editor.setValue(currentExercise.solutionCode || "");
  } else if (source === "solved") {
    editor.setValue(currentExercise.solvedCode || "");
  } else {
    editor.setValue(currentExercise.starterCode || "");
  }
  updateSourceControls();
}

async function selectProblem(section, problemId, button) {
  try {
    const problem = await api(`/sections/${encodeURIComponent(section.id)}/problems/${encodeURIComponent(problemId)}`);
    const defaultSlug = problemId.replace(/-/g, "_");
    currentExercise = {
      sectionId: section.id,
      problemId,
      starterCode: problem.starter_code,
      starterFilename: problem.starter_filename || `${defaultSlug}.c`,
      solutionCode: typeof problem.solution_code === "string" ? problem.solution_code : null,
      solutionFilename: problem.solution_filename || `${defaultSlug}_solution.c`,
      solvedCode: typeof problem.solved_code === "string" ? problem.solved_code : null,
      solvedFilename: problem.solved_filename || `${defaultSlug}_solved.c`,
      cases: Array.isArray(problem.cases) ? problem.cases : [],
      activeSource: "template",
    };
    localStorage.setItem("embedded-c-lab.last-problem", `${section.id}/${problemId}`);
    document.querySelectorAll(".problem-link").forEach((item) => item.classList.remove("active"));
    button.classList.add("active");
    const sectionGroup = button.closest(".section-group");
    const sectionToggle = sectionGroup?.querySelector(".section-toggle");
    const sectionProblems = sectionGroup?.querySelector(".section-problems");
    if (sectionToggle && sectionProblems) {
      sectionToggle.setAttribute("aria-expanded", "true");
      sectionProblems.hidden = false;
    }
    document.getElementById("lesson-section").textContent = section.name.toUpperCase();
    document.getElementById("lesson-title").textContent = problem.title;
    renderMarkdown(document.getElementById("lesson-description"), problem.description);
    document.getElementById("lesson-hint").textContent = problem.hint;
    document.getElementById("hint-panel").hidden = !problem.hint;

    const solutionPanel = document.getElementById("solution-panel");
    if (solutionPanel) {
      solutionPanel.hidden = !currentExercise.solutionCode;
    }

    document.getElementById("result-summary").textContent = "Run your solution to check it.";
    document.getElementById("result-list").replaceChildren();
    loadExerciseSource(currentExercise.activeSource);
    runButton.disabled = false;
  } catch (error) {
    showNotice(error.message);
  }
}

async function loadCourse() {
  const courseList = document.getElementById("course-list");
  const [course, savedProgress] = await Promise.all([api("/sections"), api("/progress")]);
  progress = savedProgress.completed || {};
  const problemButtons = new Map();
  for (const section of course.sections) {
    const group = document.createElement("section");
    group.className = "section-group";
    const toggle = document.createElement("button");
    toggle.type = "button";
    toggle.className = "section-toggle";
    toggle.setAttribute("aria-expanded", "false");
    const problemListId = `section-problems-${section.id}`;
    toggle.setAttribute("aria-controls", problemListId);
    const chevron = document.createElement("span");
    chevron.className = "tree-chevron";
    chevron.setAttribute("aria-hidden", "true");
    const label = document.createElement("span");
    label.className = "section-name";
    label.textContent = section.name;
    toggle.append(chevron, label);
    const problems = document.createElement("div");
    problems.id = problemListId;
    problems.className = "section-problems";
    problems.setAttribute("role", "group");
    problems.setAttribute("aria-label", section.name);
    problems.hidden = true;
    toggle.addEventListener("click", () => {
      const expanded = toggle.getAttribute("aria-expanded") === "true";
      toggle.setAttribute("aria-expanded", String(!expanded));
      problems.hidden = expanded;
    });
    group.append(toggle, problems);
    for (const problemId of section.problems) {
      const button = document.createElement("button");
      button.type = "button";
      button.className = "problem-link";
      button.dataset.problemKey = `${section.id}/${problemId}`;
      const fileIcon = document.createElement("span");
      fileIcon.className = "tree-file-icon";
      fileIcon.setAttribute("aria-hidden", "true");
      fileIcon.textContent = "C";
      const problemName = document.createElement("span");
      problemName.className = "problem-name";
      problemName.textContent = problemId.replaceAll("-", " ").replaceAll("_", " ");
      button.append(fileIcon, problemName);
      if (progress[`${section.id}/${problemId}`]) button.classList.add("completed");
      button.addEventListener("click", () => selectProblem(section, problemId, button));
      problems.append(button);
      problemButtons.set(`${section.id}/${problemId}`, { section, problemId, button });
    }
    courseList.append(group);
  }
  if (!course.sections.some((section) => section.problems.length)) {
    document.getElementById("lesson-description").textContent = "No exercises were found. Add a problem folder and list its ID in exercises/section.json.";
  }
}

document.getElementById("source-toggle").addEventListener("click", () => {
  if (!currentExercise) return;
  let nextSource = "template";
  if (currentExercise.activeSource === "template") {
    if (typeof currentExercise.solvedCode === "string") {
      nextSource = "solved";
    } else {
      return;
    }
  }
  const activeCode = currentExercise.activeSource === "solution"
    ? currentExercise.solutionCode
    : (currentExercise.activeSource === "solved" ? currentExercise.solvedCode : currentExercise.starterCode);
  if (
    editor.getValue() !== activeCode &&
    !window.confirm("Switching files will replace your current editor contents. Continue?")
  ) return;
  loadExerciseSource(nextSource);
});

const loadSolutionBtn = document.getElementById("load-solution-btn");
if (loadSolutionBtn) {
  loadSolutionBtn.addEventListener("click", () => {
    if (!currentExercise || !currentExercise.solutionCode) return;
    if (currentExercise.activeSource === "solution") {
      showNotice("The official problem solution is already loaded.");
      return;
    }
    const activeCode = currentExercise.activeSource === "solved"
      ? currentExercise.solvedCode
      : currentExercise.starterCode;
    if (
      editor.getValue() !== activeCode &&
      !window.confirm("Loading the problem solution will replace your current editor contents. Continue?")
    ) return;
    loadExerciseSource("solution");
    showNotice(`Loaded problem solution (${currentExercise.solutionFilename}).`);
  });
}

const downloadBtn = document.getElementById("download-btn");
if (downloadBtn) {
  downloadBtn.addEventListener("click", () => {
    if (!editor) return;
    const content = editor.getValue();
    const filenameEl = document.getElementById("filename");
    let filename = (filenameEl && filenameEl.textContent.trim()) || "solution.c";
    if (!filename.endsWith(".c") && !filename.endsWith(".h")) {
      filename += ".c";
    }
    const blob = new Blob([content], { type: "text/x-c;charset=utf-8" });
    const url = URL.createObjectURL(blob);
    const a = document.createElement("a");
    a.href = url;
    a.download = filename;
    document.body.appendChild(a);
    a.click();
    document.body.removeChild(a);
    URL.revokeObjectURL(url);
  });
}

const saveDiskToggle = document.getElementById("save-disk-toggle");
if (saveDiskToggle) {
  saveDiskToggle.checked = localStorage.getItem("embedded-c-lab.save-disk") === "true";
  saveDiskToggle.addEventListener("change", () => {
    localStorage.setItem("embedded-c-lab.save-disk", String(saveDiskToggle.checked));
  });
}

runButton.addEventListener("click", async () => {
  if (!currentExercise || !editor) return;
  const exercise = currentExercise;
  runButton.disabled = true;
  runButton.textContent = "Compiling...";
  document.getElementById("result-summary").textContent = "Compiling with WebAssembly (c2wasm)...";
  clearEditorMarkers();
  const submittedSource = editor.getValue();
  const resultList = document.getElementById("result-list");
  resultList.replaceChildren();

  let wasmBytes;
  try {
    wasmBytes = await CompilerAPI.compileBinary(submittedSource);
  } catch (compileErr) {
    const rawErrMsg = compileErr.message || String(compileErr);
    const formattedError = formatCompilerError(rawErrMsg, submittedSource);
    showNotice(formattedError);
    document.getElementById("result-summary").textContent = "Compilation failed.";
    const errorInfo = parseCompilerError(rawErrMsg);
    if (errorInfo) setEditorErrorMarker(errorInfo);
    runButton.disabled = false;
    runButton.innerHTML = '<span aria-hidden="true">▶</span> Run tests';
    return;
  }

  runButton.textContent = "Running tests...";
  document.getElementById("result-summary").textContent = "Running test cases in WebAssembly...";

  const cases = exercise.cases && exercise.cases.length > 0
    ? exercise.cases
    : [{ number: 1, input: "", expected: "" }];

  const results = [];
  for (const testCase of cases) {
    const exec = await runCaseWithTimeout(wasmBytes, testCase.input, 2000);
    const passed = (exec.exitCode === 0 && !exec.error) &&
      (normalizeOutput(exec.stdout) === normalizeOutput(testCase.expected));
    const testResult = {
      number: testCase.number,
      passed: passed,
      input: testCase.input,
      expected: testCase.expected,
      actual: exec.stdout || "",
      error: exec.error || exec.stderr || "",
      exit_code: exec.exitCode,
    };
    results.push(testResult);
    resultList.append(makeResultCard(testResult));
  }

  const passedCount = results.filter((item) => item.passed).length;
  const allPassed = passedCount === results.length;
  document.getElementById("result-summary").textContent = allPassed
    ? `All ${results.length} tests passed.`
    : `${passedCount} of ${results.length} tests passed.`;

  if (allPassed) {
    const saveToDisk = saveDiskToggle ? saveDiskToggle.checked === true : false;
    try {
      await api(
        `/sections/${encodeURIComponent(exercise.sectionId)}/problems/${encodeURIComponent(exercise.problemId)}/save-solved`,
        {
          method: "POST",
          headers: { "Content-Type": "application/json" },
          body: JSON.stringify({ source: submittedSource, save_file: saveToDisk }),
        }
      );
    } catch (saveErr) {
      console.warn("Could not save progress to server:", saveErr);
    }

    progress[`${exercise.sectionId}/${exercise.problemId}`] = true;
    exercise.solvedCode = submittedSource;
    exercise.activeSource = exercise === currentExercise ? "solved" : exercise.activeSource;
    if (currentExercise === exercise) updateSourceControls();
    document.querySelectorAll(".problem-link").forEach((button) => {
      if (button.dataset.problemKey === `${exercise.sectionId}/${exercise.problemId}`) {
        button.classList.add("completed");
      }
    });
  }

  runButton.disabled = false;
  runButton.innerHTML = '<span aria-hidden="true">▶</span> Run tests';
});

(async function start() {
  try {
    setupSplitters();
    if (typeof CompilerAPI !== "undefined" && CompilerAPI.init) {
      CompilerAPI.init().catch((err) => console.warn("CompilerAPI preload deferred:", err));
    }
    await loadMonaco();
    await loadCourse();
  } catch (error) {
    showNotice(error.message);
    if (!editor) {
      const fallback = document.createElement("textarea");
      fallback.className = "editor-fallback";
      fallback.setAttribute("aria-label", "C code editor (Monaco could not be loaded)");
      fallback.value = "/* Monaco could not be loaded. Check your internet connection. */\n";
      editorHost.append(fallback);
      editor = { getValue: () => fallback.value, setValue: (value) => { fallback.value = value; } };
      await loadCourse().catch((courseError) => showNotice(courseError.message));
    }
  }
})();
