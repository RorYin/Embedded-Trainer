from __future__ import annotations

import os
import shutil
import stat
import subprocess
import tempfile
import urllib.request
import zipfile
from pathlib import Path, PurePosixPath


PROJECT_ROOT = Path(__file__).resolve().parents[1]
COMPILER_DESTINATION = Path(
    os.environ.get("EMBEDDED_C_LAB_COMPILER_DIR", PROJECT_ROOT / "tools" / "gcc")
).resolve()
COMPILER_PARENT = COMPILER_DESTINATION.parent
ARCHIVE_NAME = "winlibs-x86_64-posix-seh-gcc-16.1.0-mingw-w64msvcrt-14.0.0-r4.zip"
DOWNLOAD_URL = (
    "https://github.com/brechtsanders/winlibs_mingw/releases/download/"
    "16.1.0posix-14.0.0-msvcrt-r4/"
    f"{ARCHIVE_NAME}"
)
ARCHIVE_ROOT = "mingw64/"


def main() -> None:
    gcc = COMPILER_DESTINATION / "bin" / "gcc.exe"
    if gcc.is_file():
        print(f"Portable GCC is already installed at {gcc}.")
        return
    if COMPILER_DESTINATION.exists():
        raise FileExistsError(
            f"{COMPILER_DESTINATION} exists but does not contain bin/gcc.exe. "
            "Move or inspect that folder before installing GCC."
        )

    COMPILER_PARENT.mkdir(parents=True, exist_ok=True)
    archive = COMPILER_PARENT / ARCHIVE_NAME
    downloaded_here = not archive.is_file()
    if downloaded_here:
        print("Downloading the official 64-bit WinLibs GCC 16.1.0 portable release...")
        urllib.request.urlretrieve(DOWNLOAD_URL, archive)

    staging = Path(tempfile.mkdtemp(prefix=".gcc-stage-", dir=COMPILER_PARENT))
    installed = False
    try:
        with zipfile.ZipFile(archive) as package:
            bad_member = package.testzip()
            if bad_member is not None:
                raise zipfile.BadZipFile(f"Corrupt GCC archive member: {bad_member}")

            for member in package.infolist():
                if member.filename == ARCHIVE_ROOT:
                    continue
                if not member.filename.startswith(ARCHIVE_ROOT):
                    continue
                relative_name = member.filename[len(ARCHIVE_ROOT) :]
                relative_path = PurePosixPath(relative_name)
                if (
                    not relative_name
                    or relative_path.is_absolute()
                    or ".." in relative_path.parts
                    or "\\" in relative_name
                ):
                    raise ValueError(f"Unsafe path in GCC archive: {member.filename}")
                unix_mode = member.external_attr >> 16
                if stat.S_ISLNK(unix_mode):
                    raise ValueError(f"Unexpected symbolic link in GCC archive: {member.filename}")

                destination = staging.joinpath(*relative_path.parts)
                if member.is_dir():
                    destination.mkdir(parents=True, exist_ok=True)
                    continue
                destination.parent.mkdir(parents=True, exist_ok=True)
                with package.open(member) as source, destination.open("wb") as target:
                    shutil.copyfileobj(source, target)

        staged_gcc = staging / "bin" / "gcc.exe"
        if not staged_gcc.is_file():
            raise FileNotFoundError(f"The WinLibs archive did not contain {ARCHIVE_ROOT}bin/gcc.exe.")
        environment = os.environ.copy()
        environment["PATH"] = str(staging / "bin") + os.pathsep + environment.get("PATH", "")
        result = subprocess.run(
            [str(staged_gcc), "--version"],
            capture_output=True,
            text=True,
            encoding="utf-8",
            errors="replace",
            timeout=30,
            env=environment,
            check=False,
        )
        if result.returncode:
            raise RuntimeError(f"Downloaded GCC failed its startup check: {result.stderr}")

        staging.replace(COMPILER_DESTINATION)
        installed = True
        print(result.stdout.splitlines()[0])
        print(f"Portable GCC installed at {COMPILER_DESTINATION}.")
    finally:
        if staging.exists():
            shutil.rmtree(staging)
        if installed and archive.exists():
            archive.unlink()


if __name__ == "__main__":
    main()
