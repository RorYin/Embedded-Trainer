from __future__ import annotations

import shutil
import tarfile
import tempfile
import urllib.request
from pathlib import Path, PurePosixPath


VERSION = "0.52.2"
URL = f"https://registry.npmjs.org/monaco-editor/-/monaco-editor-{VERSION}.tgz"
DESTINATION = Path(__file__).resolve().parents[1] / "static" / "vendor" / "monaco"
ARCHIVE_PREFIX = "package/min/vs/"


def main() -> None:
    loader = DESTINATION / "min" / "vs" / "loader.js"
    if loader.is_file():
        print(f"Monaco {VERSION} is already available at {DESTINATION}.")
        return

    DESTINATION.mkdir(parents=True, exist_ok=True)
    with tempfile.TemporaryDirectory(prefix="embedded-c-lab-monaco-") as temp_name:
        archive_path = Path(temp_name) / "monaco-editor.tgz"
        print(f"Downloading Monaco Editor {VERSION}...")
        urllib.request.urlretrieve(URL, archive_path)
        with tarfile.open(archive_path, mode="r:gz") as archive:
            for member in archive:
                if not member.name.startswith(ARCHIVE_PREFIX):
                    continue
                relative_name = member.name[len(ARCHIVE_PREFIX) :]
                relative_path = PurePosixPath(relative_name)
                if not relative_name or relative_path.is_absolute() or ".." in relative_path.parts:
                    raise ValueError(f"Unexpected path in Monaco package: {member.name}")
                target_path = DESTINATION / "min" / "vs" / Path(*relative_path.parts)
                if member.isdir():
                    target_path.mkdir(parents=True, exist_ok=True)
                elif member.isfile():
                    target_path.parent.mkdir(parents=True, exist_ok=True)
                    source = archive.extractfile(member)
                    if source is None:
                        raise OSError(f"Could not extract Monaco file: {member.name}")
                    with source, target_path.open("wb") as destination:
                        shutil.copyfileobj(source, destination)

    if not loader.is_file():
        raise FileNotFoundError(f"Monaco package did not contain {loader}.")
    print(f"Monaco {VERSION} is ready.")


if __name__ == "__main__":
    main()
