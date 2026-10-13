# Embedded C Lab

A local, self-paced Embedded C exercise app. A Windows executable starts a FastAPI server on `127.0.0.1:8765` and opens a Monaco-based coding page in your browser. The release consists of the small web-app executable, `tools/gcc/` with the portable compiler, and `lessons/` with editable exercises and saved progress.

## Run from source

1. Install Python 3.11 or newer.
2. In PowerShell, from this folder:

   ```powershell
   python -m venv .venv
   .\.venv\Scripts\python.exe -m pip install -r requirements.txt
   .\.venv\Scripts\python.exe scripts\download_gcc.py
   .\.venv\Scripts\python.exe main.py
   ```

3. The app opens at <http://127.0.0.1:8765/embedded-c-lab/>.

The portable compiler installs in the project at `tools/gcc/`. Keep this folder next to the executable when sharing or running the packaged app. The app can also use GCC on `PATH` or the `EMBEDDED_C_LAB_GCC` environment variable if `tools/gcc/` is missing.

If you share the app without the large compiler folder, include `tools/download_gcc.bat` instead. Run it on the target Windows PC while connected to the internet; it downloads and installs this project's pinned WinLibs GCC 16.1.0 into `tools/gcc/`. Python is not required for this download. The compiler archive itself is removed after a successful install.

`build.bat` bundles Monaco 0.52.2 into the executable, so the packaged app's editor works offline. GCC stays outside the executable in `tools/gcc/`. For offline source development, run `.\.venv\Scripts\python.exe scripts\download_monaco.py`; source mode also falls back to loading Monaco from jsDelivr.

## Add an exercise

Create a problem folder under a section folder, add its files, then list the folder ID in `lessons/section.json`:

```text
lessons/
  section.json
  basics/
    hello-world/
      problem.c
      problem_description.json
      solution.c
      input_and_output.json
    sum-two-numbers/
      problem.c
      problem_description.json
      solution.c
      input_and_output.json       # May contain multiple cases
      problem_solved.c           # Created after all tests pass
  progress.json              # Created automatically when progress is saved
```

`section.json` format:

```json
{
  "sections": [
    {
      "id": "basics",
      "name": "Basics",
      "problems": ["hello-world"]
    }
  ]
}
```

The `id` values are folder names and must contain only letters, numbers, underscores, or hyphens. A problem's `problem_description.json` has `title`, `description`, and `hint` strings. Its `problem.c` is the editor's starter code; `solution.c` is an optional reference answer and is never sent to the browser.

Test cases are fed to the compiled program on standard input and compared with standard output:

```json
{
  "cases": [
    {
      "input": "5\n",
      "output": "25\n"
    }
  ]
}
```

Output comparison normalizes Windows/Unix newlines and ignores leading/trailing whitespace. Add between one and 20 cases per problem. Restart the app after changing lessons. Completion status is saved in `lessons/progress.json`. When every test passes, the current editor contents are saved beside `problem.c` as `problem_solved.c`. Completed status is retained if a later run fails.

## Build the Windows executable

1. Install Python 3.11 or newer for Windows and ensure the build machine has internet access for Python packages, Monaco, and the portable WinLibs GCC download. Alternatively, download GCC separately by running `tools\download_gcc.bat`.
2. Double-click `build.bat`. It downloads WinLibs GCC 16.1.0 into `tools/gcc/`, installs the Python requirements into `.venv`, and creates `dist/EmbeddedCLab.exe` with the matching icon.
3. Copy or share the complete `dist/` folder. It contains the small `EmbeddedCLab.exe`, `tools/gcc/`, and `lessons/` folders.

The executable is a PyInstaller **one-file** build of the Python web server and its static frontend. GCC is deliberately not bundled into it; the portable toolchain is kept in `tools/gcc/` and copied beside the executable as `dist/tools/gcc/`. Exercises and progress stay in the separate `dist/lessons/` folder. If you zip the release to share it, include all three: the EXE, `tools/`, and `lessons/`.

The portable compiler is the 64-bit WinLibs GCC 16.1.0 POSIX/SEH/MSVCRT release. Its upstream license and runtime files remain in `tools/gcc/`; include these files and observe the applicable GCC and MinGW-w64 redistribution terms when distributing builds.

The executable icon is generated to match the code-bracket logo shown in the application header.

## Interface layout

The interface follows VS Code Dark+: a charcoal `#1e1e1e` editor and results area, `#252526` side panels and toolbars, navy `#094771` active lesson selection, `#007acc` focus accents, and subtle `#2b2b2b` separators. Monaco uses distinct Dark+ syntax colors for keywords, strings, numbers, identifiers, and comments. The problem description and Monaco editor are on the left and center, the lesson tree is in a right-side panel, and test results occupy a full-width bottom panel. Drag the vertical dividers to resize the description and lesson tree, and drag the horizontal divider above results to adjust the bottom panel height. On narrow screens, these panels stack vertically and their dividers adjust their heights. Sizes are remembered in the browser, and the description can be collapsed to give the editor more space.

The problem description starts expanded with added spacing for comfortable reading. Use the arrow button to tuck it into a narrow side strip while solving; the arrow points toward the direction the panel will collapse or expand.

Problem descriptions in `problem_description.json` are rendered as Markdown previews. Supported formatting includes headings, paragraphs, lists, fenced code blocks, inline code, bold, italic, and safe HTTP(S) links. Use fenced `text` code blocks to show example input and expected output without exposing hidden test cases.

## Test-case results

`input_and_output.json` can contain multiple entries in its `cases` array. The solution is compiled once per run and executed separately with every case's input. Every result has its own expandable output panel: failed tests open by default; passed tests can be expanded to compare the program's output with the expected output.

The app remembers the last selected exercise in the browser and reopens it after reload. It loads `problem.c` first, even for a completed exercise. When `problem_solved.c` exists and progress marks the exercise complete, the editor toolbar offers **Load solved solution**; the same button toggles back to **Load template**.

## Safety and limits

This app binds only to loopback (`127.0.0.1`) and does not expose its endpoints to your network. Compiled submissions run as your Windows user, **not in a security sandbox**. A time limit stops ordinary programs that hang, but it does not prevent code from accessing or changing files, launching processes, or using system resources. Only run code you trust; use a properly isolated virtual machine for untrusted submissions. The browser-based editor does not require an installed VS Code.
