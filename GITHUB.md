# GitHub guide

This repository contains the source for Embedded C Lab, a local-first Embedded C practice app.

## Clone and start

1. Install Python 3.11 or newer and Git.
2. Clone this repository.
3. In PowerShell, in the cloned directory, run:

   ```powershell
   python -m venv .venv
   .\.venv\Scripts\python.exe -m pip install -r requirements-dev.txt
   .\.venv\Scripts\python.exe scripts\download_gcc.py
   .\.venv\Scripts\python.exe -m pytest
   .\.venv\Scripts\python.exe main.py
   ```

4. Open <http://127.0.0.1:8765/embedded-c-lab/> if it did not open automatically.

Monaco is loaded from jsDelivr by default during source development. `build.bat` downloads and bundles Monaco into the executable; the portable GCC remains a separate `tools/gcc/` folder. See [README.md](README.md) for the exercise JSON format and compiler/build details.
For a packaged release shared without GCC, run `tools\download_gcc.bat` on Windows to download and install the project's pinned WinLibs GCC 16.1.0 release into `tools\gcc\`; Python is not needed for this step.

The desktop UI uses three resizable panes for the lesson tree, editor, and test results. Problem descriptions support Markdown previews with fenced example input/output blocks.

## Build a Windows release

On Windows, install Python and ensure the build machine can download Python packages, Monaco, and WinLibs GCC. Run `build.bat`. Share `dist/` with `EmbeddedCLab.exe`, `tools/download_gcc.bat`, and editable `lessons/` with saved progress. Recipients can run the downloader to fetch `tools/gcc/` on demand instead of sharing the large compiler folder.

## Contribute exercises

Add a problem directory under `lessons/<section-id>/<problem-id>/` containing `problem.c`, `problem_description.json`, `input_and_output.json`, and optionally `solution.c`. Add the problem ID to the matching section in `lessons/section.json`. Keep reference solutions out of frontend/API responses. Exercise IDs use letters, numbers, hyphens, or underscores. Completion progress is stored at `lessons/progress.json`.

## Security note

This is a single-user, loopback-only learning application, not a hosted coding judge. Submitted C programs execute with the local user's permissions. Do not run untrusted submissions or expose the service to a network.
