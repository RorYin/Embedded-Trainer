@echo off
setlocal
cd /d "%~dp0"

where python >nul 2>nul
if errorlevel 1 (
    echo Python was not found. Install Python 3.11 or newer and add it to PATH.
    exit /b 1
)

if not exist ".venv\Scripts\python.exe" (
    python -m venv .venv
    if errorlevel 1 exit /b 1
)

".venv\Scripts\python.exe" -m pip install --upgrade pip
if errorlevel 1 exit /b 1
".venv\Scripts\python.exe" -m pip install -r requirements.txt
if errorlevel 1 exit /b 1
".venv\Scripts\python.exe" scripts\download_monaco.py
if errorlevel 1 exit /b 1
".venv\Scripts\python.exe" -m PyInstaller --noconfirm --clean --onefile --name EmbeddedCLab --icon "assets\EmbeddedCLab.ico" --add-data "static;static" main.py
if errorlevel 1 exit /b 1

xcopy "lessons" "dist\lessons" /E /I /Y >nul
echo.
echo Build complete: dist\EmbeddedCLab.exe
echo In-browser WebAssembly compiler: bundled in static assets
echo Editable lessons and progress: dist\lessons
endlocal
