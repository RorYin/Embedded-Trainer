from __future__ import annotations

import json
import os
import re
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path
from typing import Annotated, Any

from fastapi import FastAPI, HTTPException
from fastapi.responses import FileResponse, RedirectResponse
from fastapi.staticfiles import StaticFiles
from pydantic import BaseModel, Field


APP_NAME = "Embedded C Lab"
BASE_PATH = "/embedded-c-lab"
API_PATH = f"{BASE_PATH}/api/v1"
HOST = "127.0.0.1"
PORT = 8765
MAX_SOURCE_LENGTH = 128_000
MAX_TEST_CASES = 20
COMPILE_TIMEOUT_SECONDS = 15
RUN_TIMEOUT_SECONDS = 2
SAFE_ID = re.compile(r"^[a-zA-Z0-9_-]+$")
SAVED_SOLUTION_NAME = "problem_solved.c"


def application_root() -> Path:
    if getattr(sys, "frozen", False):
        return Path(sys.executable).resolve().parent
    return Path(__file__).resolve().parent


def resource_root() -> Path:
    bundled_root = getattr(sys, "_MEIPASS", None)
    return Path(bundled_root) if bundled_root else Path(__file__).resolve().parent


def lessons_root() -> Path:
    return application_root() / "lessons"


def compiler_path() -> Path | None:
    external_root = application_root() / "tools" / "gcc"
    external_candidates = [external_root / "bin" / "gcc.exe", external_root / "gcc.exe"]
    external_compiler = next((candidate for candidate in external_candidates if candidate.is_file()), None)
    if external_compiler:
        return external_compiler
    configured = os.environ.get("EMBEDDED_C_LAB_GCC")
    if configured and Path(configured).is_file():
        return Path(configured)
    configured_root = os.environ.get("EMBEDDED_C_LAB_COMPILER_DIR")
    if configured_root:
        configured_compiler = Path(configured_root) / "bin" / "gcc.exe"
        if configured_compiler.is_file():
            return configured_compiler
    local_app_data = os.environ.get("LOCALAPPDATA")
    if local_app_data:
        portable_compiler = (
            Path(local_app_data) / "EmbeddedCLab" / "compiler" / "gcc" / "bin" / "gcc.exe"
        )
        if portable_compiler.is_file():
            return portable_compiler
    system_compiler = shutil.which("gcc.exe") or shutil.which("gcc")
    return Path(system_compiler) if system_compiler else None


def read_json(path: Path) -> Any:
    try:
        return json.loads(path.read_text(encoding="utf-8"))
    except (OSError, json.JSONDecodeError) as exc:
        raise HTTPException(status_code=500, detail=f"Could not read {path.name}: {exc}") from exc


def exercise_directory(section_id: str, problem_id: str) -> Path:
    if not SAFE_ID.fullmatch(section_id) or not SAFE_ID.fullmatch(problem_id):
        raise HTTPException(status_code=400, detail="Invalid section or problem identifier.")
    directory = lessons_root() / section_id / problem_id
    if not directory.is_dir():
        raise HTTPException(status_code=404, detail="Exercise not found.")
    return directory


def read_progress() -> dict[str, Any]:
    progress_path = lessons_root() / "progress.json"
    if not progress_path.exists():
        return {"completed": {}}
    progress = read_json(progress_path)
    if not isinstance(progress, dict) or not isinstance(progress.get("completed", {}), dict):
        raise HTTPException(status_code=500, detail="progress.json must contain a completed object.")
    return {**progress, "completed": progress.get("completed", {})}


def solved_solution_path(directory: Path) -> Path:
    solution_path = directory / SAVED_SOLUTION_NAME
    if solution_path.is_symlink():
        raise HTTPException(status_code=500, detail=f"{SAVED_SOLUTION_NAME} must not be a symbolic link.")
    return solution_path


def save_solved_solution(directory: Path, source: str) -> None:
    solution_path = solved_solution_path(directory)
    temporary_path: Path | None = None
    try:
        with tempfile.NamedTemporaryFile(
            mode="w",
            encoding="utf-8",
            dir=directory,
            prefix=f".{SAVED_SOLUTION_NAME}.",
            suffix=".tmp",
            delete=False,
        ) as temporary_file:
            temporary_path = Path(temporary_file.name)
            temporary_file.write(source)
        temporary_path.replace(solution_path)
    except OSError as exc:
        if temporary_path and temporary_path.exists():
            temporary_path.unlink(missing_ok=True)
        raise HTTPException(status_code=500, detail=f"Could not save {SAVED_SOLUTION_NAME}: {exc}") from exc


def persist_progress(section_id: str, problem_id: str, passed: bool) -> None:
    progress_path = lessons_root() / "progress.json"
    try:
        progress_path.parent.mkdir(parents=True, exist_ok=True)
        progress = json.loads(progress_path.read_text(encoding="utf-8")) if progress_path.exists() else {}
        completed = progress.setdefault("completed", {})
        key = f"{section_id}/{problem_id}"
        completed[key] = bool(completed.get(key, False) or passed)
        temporary_path = progress_path.with_suffix(".tmp")
        temporary_path.write_text(json.dumps(progress, indent=2) + "\n", encoding="utf-8")
        temporary_path.replace(progress_path)
    except (OSError, json.JSONDecodeError, AttributeError) as exc:
        raise HTTPException(status_code=500, detail=f"Could not save progress: {exc}") from exc


def normalize_output(value: str) -> str:
    return value.replace("\r\n", "\n").replace("\r", "\n").strip()


class RunRequest(BaseModel):
    source: Annotated[str, Field(max_length=MAX_SOURCE_LENGTH)]


class ProgressRequest(BaseModel):
    section_id: Annotated[str, Field(pattern=r"^[a-zA-Z0-9_-]+$")]
    problem_id: Annotated[str, Field(pattern=r"^[a-zA-Z0-9_-]+$")]
    passed: bool


app = FastAPI(title=APP_NAME, docs_url=None, redoc_url=None, openapi_url=None)
static_dir = resource_root() / "static"
app.mount(f"{BASE_PATH}/static", StaticFiles(directory=static_dir), name="static")


@app.get("/")
def redirect_to_app() -> RedirectResponse:
    return RedirectResponse(url=f"{BASE_PATH}/")


@app.get(f"{BASE_PATH}/")
def index() -> FileResponse:
    return FileResponse(static_dir / "index.html")


@app.get(f"{API_PATH}/sections")
def list_sections() -> dict[str, Any]:
    sections_file = lessons_root() / "section.json"
    payload = read_json(sections_file)
    sections = payload.get("sections") if isinstance(payload, dict) else None
    if not isinstance(sections, list):
        raise HTTPException(status_code=500, detail="section.json must contain a sections array.")

    result = []
    for section in sections:
        if not isinstance(section, dict) or not SAFE_ID.fullmatch(str(section.get("id", ""))):
            raise HTTPException(status_code=500, detail="Each section needs a safe id and a name.")
        problem_ids = section.get("problems", [])
        if not isinstance(problem_ids, list) or any(
            not isinstance(problem_id, str) or not SAFE_ID.fullmatch(problem_id)
            for problem_id in problem_ids
        ):
            raise HTTPException(
                status_code=500,
                detail=f"Section {section['id']} must contain a list of safe problem IDs.",
            )
        result.append(
            {
                "id": section["id"],
                "name": section.get("name", section["id"]),
                "problems": problem_ids,
            }
        )
    return {"sections": result}


@app.get(f"{API_PATH}/sections/{{section_id}}/problems/{{problem_id}}")
def get_problem(section_id: str, problem_id: str) -> dict[str, Any]:
    directory = exercise_directory(section_id, problem_id)
    description = read_json(directory / "problem_description.json")
    if not isinstance(description, dict):
        raise HTTPException(status_code=500, detail="problem_description.json must contain an object.")
    try:
        starter_code = (directory / "problem.c").read_text(encoding="utf-8")
    except OSError as exc:
        raise HTTPException(status_code=500, detail=f"Could not read problem.c: {exc}") from exc
    completed = read_progress()["completed"].get(f"{section_id}/{problem_id}") is True
    solved_code = None
    if completed:
        saved_solution = solved_solution_path(directory)
        if saved_solution.is_file():
            try:
                solved_code = saved_solution.read_text(encoding="utf-8")
            except (OSError, UnicodeError) as exc:
                raise HTTPException(
                    status_code=500,
                    detail=f"Could not read {SAVED_SOLUTION_NAME}: {exc}",
                ) from exc
    return {
        "id": problem_id,
        "section_id": section_id,
        "title": description.get("title", problem_id),
        "description": description.get("description", ""),
        "hint": description.get("hint", ""),
        "starter_code": starter_code,
        "completed": completed,
        "solved_code": solved_code,
    }


@app.post(f"{API_PATH}/sections/{{section_id}}/problems/{{problem_id}}/run")
def run_problem(section_id: str, problem_id: str, request: RunRequest) -> dict[str, Any]:
    directory = exercise_directory(section_id, problem_id)
    cases_data = read_json(directory / "input_and_output.json")
    cases = cases_data.get("cases") if isinstance(cases_data, dict) else None
    if not isinstance(cases, list) or not 1 <= len(cases) <= MAX_TEST_CASES:
        raise HTTPException(
            status_code=500,
            detail=f"input_and_output.json must contain 1 to {MAX_TEST_CASES} cases.",
        )
    for case in cases:
        if not isinstance(case, dict) or not isinstance(case.get("input"), str) or not isinstance(
            case.get("output"), str
        ):
            raise HTTPException(
                status_code=500,
                detail="Each test case must contain string input and output fields.",
            )

    gcc = compiler_path()
    if gcc is None:
        raise HTTPException(
            status_code=503,
            detail="GCC was not found. Add the portable compiler at tools/gcc or build it with build.bat.",
        )

    results: list[dict[str, Any]] = []
    with tempfile.TemporaryDirectory(prefix="embedded-c-lab-") as temp_name:
        temp_dir = Path(temp_name)
        source_path = temp_dir / "submission.c"
        executable_path = temp_dir / "submission.exe"
        source_path.write_text(request.source, encoding="utf-8")

        environment = os.environ.copy()
        environment["PATH"] = str(gcc.parent) + os.pathsep + environment.get("PATH", "")
        try:
            compiler_options = [str(gcc)]
            specs = subprocess.run(
                [str(gcc), "-dumpspecs"],
                capture_output=True,
                text=True,
                encoding="utf-8",
                errors="replace",
                timeout=COMPILE_TIMEOUT_SECONDS,
                cwd=temp_dir,
                env=environment,
                check=False,
            )
            if specs.returncode:
                raise HTTPException(
                    status_code=500,
                    detail=f"Could not inspect GCC configuration: {specs.stdout}{specs.stderr}",
                )
            if "default-manifest.o%s" in specs.stdout:
                specs_path = temp_dir / "embedded-c-lab-gcc.specs"
                specs_path.write_text(
                    specs.stdout.replace("default-manifest.o%s", "-l:default-manifest.o"),
                    encoding="utf-8",
                )
                compiler_options.append(f"-specs={specs_path}")
            compilation = subprocess.run(
                [
                    *compiler_options,
                    "-std=c11",
                    "-Wall",
                    "-Wextra",
                    "-O0",
                    str(source_path),
                    "-o",
                    str(executable_path),
                ],
                capture_output=True,
                text=True,
                encoding="utf-8",
                errors="replace",
                timeout=COMPILE_TIMEOUT_SECONDS,
                cwd=temp_dir,
                env=environment,
                check=False,
            )
        except subprocess.TimeoutExpired:
            raise HTTPException(status_code=408, detail="Compilation exceeded its time limit.")
        except OSError as exc:
            raise HTTPException(status_code=500, detail=f"Could not start GCC: {exc}") from exc

        if compilation.returncode:
            return {
                "status": "compile_error",
                "passed": False,
                "compiler_output": compilation.stdout + compilation.stderr,
                "cases": [],
            }

        for index, case in enumerate(cases, start=1):
            try:
                execution = subprocess.run(
                    [str(executable_path)],
                    input=case["input"],
                    capture_output=True,
                    text=True,
                    encoding="utf-8",
                    errors="replace",
                    timeout=RUN_TIMEOUT_SECONDS,
                    cwd=temp_dir,
                    env=environment,
                    check=False,
                )
                actual_output = execution.stdout
                passed = (
                    execution.returncode == 0
                    and normalize_output(actual_output) == normalize_output(case["output"])
                )
                results.append(
                    {
                        "number": index,
                        "passed": passed,
                        "input": case["input"],
                        "expected": case["output"],
                        "actual": actual_output,
                        "error": execution.stderr,
                        "exit_code": execution.returncode,
                    }
                )
            except subprocess.TimeoutExpired as exc:
                output = exc.stdout or ""
                if isinstance(output, bytes):
                    output = output.decode("utf-8", errors="replace")
                results.append(
                    {
                        "number": index,
                        "passed": False,
                        "input": case["input"],
                        "expected": case["output"],
                        "actual": output,
                        "error": f"Execution exceeded the {RUN_TIMEOUT_SECONDS}-second time limit.",
                        "exit_code": None,
                    }
                )
            except OSError as exc:
                raise HTTPException(
                    status_code=500,
                    detail=f"Could not start the compiled program. Check the GCC runtime DLLs: {exc}",
                ) from exc

    passed_count = sum(case["passed"] for case in results)
    passed = passed_count == len(results)
    if passed:
        save_solved_solution(directory, request.source)
        persist_progress(section_id, problem_id, True)
    return {
        "status": "passed" if passed else "failed",
        "passed": passed,
        "compiler_output": compilation.stdout + compilation.stderr,
        "cases": results,
    }


@app.post(f"{API_PATH}/progress")
def save_progress(request: ProgressRequest) -> dict[str, Any]:
    exercise_directory(request.section_id, request.problem_id)
    persist_progress(request.section_id, request.problem_id, request.passed)
    return {"saved": True}


@app.get(f"{API_PATH}/progress")
def get_progress() -> dict[str, Any]:
    progress = read_progress()
    return {"completed": progress["completed"]}
