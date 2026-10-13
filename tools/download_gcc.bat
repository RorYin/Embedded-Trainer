@echo off
setlocal

set "GCC_DIR=%~dp0gcc"
set "GCC_STAGE=%~dp0.gcc-download-stage"
set "GCC_ARCHIVE=%TEMP%\EmbeddedCLab-WinLibs-GCC-16.1.0.zip"
set "GCC_URL=https://github.com/brechtsanders/winlibs_mingw/releases/download/16.1.0posix-14.0.0-msvcrt-r4/winlibs-x86_64-posix-seh-gcc-16.1.0-mingw-w64msvcrt-14.0.0-r4.zip"

if exist "%GCC_DIR%\bin\gcc.exe" (
    echo Portable GCC is already installed at "%GCC_DIR%".
    exit /b 0
)

if exist "%GCC_DIR%" (
    echo ERROR: "%GCC_DIR%" already exists but gcc.exe is missing.
    echo Inspect or move that folder before trying again.
    exit /b 1
)

if exist "%GCC_STAGE%" (
    echo ERROR: Temporary folder "%GCC_STAGE%" already exists.
    echo Inspect or remove that folder before trying again.
    exit /b 1
)

echo Downloading the project's pinned WinLibs GCC 16.1.0 release...
powershell.exe -NoProfile -ExecutionPolicy Bypass -Command "$ErrorActionPreference = 'Stop'; $ProgressPreference = 'SilentlyContinue'; try { Invoke-WebRequest -Uri $env:GCC_URL -OutFile $env:GCC_ARCHIVE; Expand-Archive -LiteralPath $env:GCC_ARCHIVE -DestinationPath $env:GCC_STAGE; $stagedGcc = Join-Path $env:GCC_STAGE 'mingw64\bin\gcc.exe'; if (-not (Test-Path $stagedGcc)) { throw 'The downloaded archive did not contain mingw64\bin\gcc.exe.' }; Move-Item -LiteralPath (Join-Path $env:GCC_STAGE 'mingw64') -Destination $env:GCC_DIR; $gcc = Join-Path $env:GCC_DIR 'bin\gcc.exe'; $env:PATH = (Join-Path $env:GCC_DIR 'bin') + ';' + $env:PATH; & $gcc --version; if ($LASTEXITCODE -ne 0) { throw 'The installed GCC failed its startup check.' }; Remove-Item -LiteralPath $env:GCC_ARCHIVE -Force; Remove-Item -LiteralPath $env:GCC_STAGE -Recurse -Force } catch { if (Test-Path $env:GCC_STAGE) { Remove-Item -LiteralPath $env:GCC_STAGE -Recurse -Force }; if (Test-Path $env:GCC_ARCHIVE) { Remove-Item -LiteralPath $env:GCC_ARCHIVE -Force }; throw }"
if errorlevel 1 (
    echo.
    echo ERROR: GCC could not be downloaded or installed.
    echo Check your internet connection, disk space, and PowerShell permissions.
    exit /b 1
)

echo.
echo GCC installed at "%GCC_DIR%".
echo You can now run EmbeddedCLab.exe with this tools folder beside it.
exit /b 0
