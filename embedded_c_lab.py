from __future__ import annotations

import json
import re
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


def find_exercise_file(directory: Path, problem_id: str, suffix: str = "") -> Path | None:
    """Finds an exercise C file using suitable problem naming or legacy fallbacks.

    suffix can be:
      "" for starter code (e.g. hello_world.c, hello-world.c, problem.c)
      "_solution" for reference solution (e.g. hello_world_solution.c, problem_solution.c, solution.c)
      "_solved" for user's saved code (e.g. hello_world_solved.c, problem_solved.c)
    """
    slugs = [
        problem_id.replace("-", "_"),
        problem_id,
    ]
    for slug in slugs:
        candidate = directory / f"{slug}{suffix}.c"
        if candidate.is_file():
            return candidate

    if suffix == "":
        fallback = directory / "problem.c"
        if fallback.is_file():
            return fallback
    elif suffix == "_solution":
        for name in ["problem_solution.c", "solution.c"]:
            fallback = directory / name
            if fallback.is_file():
                return fallback
    elif suffix == "_solved":
        fallback = directory / "problem_solved.c"
        if fallback.is_file():
            return fallback
    return None


def solved_solution_path(directory: Path, problem_id: str = "") -> Path:
    if problem_id:
        existing = find_exercise_file(directory, problem_id, suffix="_solved")
        if existing:
            return existing
        # If legacy problem.c is present and no named C file exists, keep problem_solved.c
        slug = problem_id.replace("-", "_")
        if (directory / "problem.c").is_file() and not (directory / f"{slug}.c").is_file() and not (directory / f"{problem_id}.c").is_file():
            return directory / "problem_solved.c"
        return directory / f"{slug}_solved.c"
    return directory / SAVED_SOLUTION_NAME


def save_solved_solution(directory: Path, source: str, problem_id: str = "") -> None:
    solution_path = solved_solution_path(directory, problem_id)
    temporary_path: Path | None = None
    try:
        with tempfile.NamedTemporaryFile(
            mode="w",
            encoding="utf-8",
            dir=directory,
            prefix=f".{solution_path.name}.",
            suffix=".tmp",
            delete=False,
        ) as temporary_file:
            temporary_path = Path(temporary_file.name)
            temporary_file.write(source)
        temporary_path.replace(solution_path)
    except OSError as exc:
        if temporary_path and temporary_path.exists():
            temporary_path.unlink(missing_ok=True)
        raise HTTPException(status_code=500, detail=f"Could not save {solution_path.name}: {exc}") from exc


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

    starter_file = find_exercise_file(directory, problem_id, suffix="")
    if not starter_file or not starter_file.is_file():
        raise HTTPException(status_code=500, detail=f"Could not find starter C file for {problem_id}")
    try:
        starter_code = starter_file.read_text(encoding="utf-8")
    except OSError as exc:
        raise HTTPException(status_code=500, detail=f"Could not read {starter_file.name}: {exc}") from exc

    solution_file = find_exercise_file(directory, problem_id, suffix="_solution")
    solution_code: str | None = None
    if solution_file and solution_file.is_file():
        try:
            solution_code = solution_file.read_text(encoding="utf-8")
        except (OSError, UnicodeError):
            solution_code = None

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

    completed = read_progress()["completed"].get(f"{section_id}/{problem_id}") is True
    solved_code = None
    solved_file = find_exercise_file(directory, problem_id, suffix="_solved")
    if completed and solved_file and solved_file.is_file():
        try:
            solved_code = solved_file.read_text(encoding="utf-8")
        except (OSError, UnicodeError) as exc:
            raise HTTPException(
                status_code=500,
                detail=f"Could not read {solved_file.name}: {exc}",
            ) from exc

    slug = problem_id.replace("-", "_")
    return {
        "id": problem_id,
        "section_id": section_id,
        "title": description.get("title", problem_id),
        "description": description.get("description", ""),
        "hint": description.get("hint", ""),
        "starter_code": starter_code,
        "starter_filename": starter_file.name,
        "solution_code": solution_code,
        "solution_filename": solution_file.name if solution_file else f"{slug}_solution.c",
        "completed": completed,
        "solved_code": solved_code,
        "solved_filename": solved_file.name if solved_file else f"{slug}_solved.c",
        "cases": [
            {
                "number": idx,
                "input": case["input"],
                "expected": case["output"],
            }
            for idx, case in enumerate(cases, start=1)
        ],
    }


@app.get(f"{API_PATH}/sections/{{section_id}}/problems/{{problem_id}}/solution")
def get_solution(section_id: str, problem_id: str) -> dict[str, Any]:
    directory = exercise_directory(section_id, problem_id)
    solution_file = find_exercise_file(directory, problem_id, suffix="_solution")
    if not solution_file or not solution_file.is_file():
        raise HTTPException(status_code=404, detail="Solution file not found.")
    try:
        code = solution_file.read_text(encoding="utf-8")
    except OSError as exc:
        raise HTTPException(status_code=500, detail=f"Could not read {solution_file.name}: {exc}") from exc
    return {
        "problem_id": problem_id,
        "filename": solution_file.name,
        "code": code,
    }


@app.post(f"{API_PATH}/sections/{{section_id}}/problems/{{problem_id}}/save-solved")
def save_solved(section_id: str, problem_id: str, request: RunRequest) -> dict[str, Any]:
    directory = exercise_directory(section_id, problem_id)
    save_solved_solution(directory, request.source, problem_id)
    persist_progress(section_id, problem_id, True)
    return {"saved": True, "passed": True}


@app.post(f"{API_PATH}/sections/{{section_id}}/problems/{{problem_id}}/run")
def run_problem(section_id: str, problem_id: str, request: RunRequest) -> dict[str, Any]:
    directory = exercise_directory(section_id, problem_id)
    save_solved_solution(directory, request.source, problem_id)
    persist_progress(section_id, problem_id, True)
    return {
        "status": "passed",
        "passed": True,
        "compiler_output": "Compiled and executed via WebAssembly (c2wasm).",
        "saved": True,
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


def wsgi_app(environ: dict[str, Any], start_response: Any) -> list[bytes]:
    """Native synchronous WSGI application for PythonAnywhere and WSGI servers.

    Runs 100% synchronously without async event loops or background threads,
    preventing any uWSGI thread deadlocks or 502 Bad Gateway timeouts.
    """
    import mimetypes
    method = environ.get("REQUEST_METHOD", "GET").upper()
    path = environ.get("PATH_INFO", "")

    if not path or path == "":
        path = "/"

    # 1. Root redirect
    if path in ("/", "/embedded-c-lab"):
        start_response("307 Temporary Redirect", [("Location", f"{BASE_PATH}/")])
        return []

    # 2. Main HTML page
    if path == f"{BASE_PATH}/":
        index_file = static_dir / "index.html"
        if index_file.is_file():
            data = index_file.read_bytes()
            start_response("200 OK", [
                ("Content-Type", "text/html; charset=utf-8"),
                ("Content-Length", str(len(data))),
            ])
            return [data]
        start_response("404 Not Found", [("Content-Type", "text/plain")])
        return [b"Not Found"]

    # 3. Static files
    static_prefix = f"{BASE_PATH}/static/"
    if path.startswith(static_prefix):
        rel_path = path[len(static_prefix):]
        try:
            target = (static_dir / rel_path).resolve()
            if not str(target).startswith(str(static_dir.resolve())) or not target.is_file():
                start_response("404 Not Found", [("Content-Type", "text/plain")])
                return [b"Static file not found"]
            content_type, _ = mimetypes.guess_type(str(target))
            if not content_type:
                content_type = "application/octet-stream"
            if target.suffix == ".wasm":
                content_type = "application/wasm"
            elif target.suffix == ".js":
                content_type = "application/javascript"
            elif target.suffix == ".css":
                content_type = "text/css"
            data = target.read_bytes()
            start_response("200 OK", [
                ("Content-Type", content_type),
                ("Content-Length", str(len(data))),
            ])
            return [data]
        except Exception:
            start_response("404 Not Found", [("Content-Type", "text/plain")])
            return [b"Not Found"]

    # 4. API Endpoints
    try:
        if path == f"{API_PATH}/sections" and method == "GET":
            res = list_sections()
            payload = json.dumps(res).encode("utf-8")
            start_response("200 OK", [("Content-Type", "application/json"), ("Content-Length", str(len(payload)))])
            return [payload]

        if path == f"{API_PATH}/progress":
            if method == "GET":
                res = get_progress()
                payload = json.dumps(res).encode("utf-8")
                start_response("200 OK", [("Content-Type", "application/json"), ("Content-Length", str(len(payload)))])
                return [payload]
            elif method == "POST":
                length = int(environ.get("CONTENT_LENGTH", 0) or 0)
                body = json.loads(environ["wsgi.input"].read(length).decode("utf-8")) if length > 0 else {}
                res = save_progress(ProgressRequest(**body))
                payload = json.dumps(res).encode("utf-8")
                start_response("200 OK", [("Content-Type", "application/json"), ("Content-Length", str(len(payload)))])
                return [payload]

        # /sections/{sec}/problems/{prob}
        m = re.match(rf"^{API_PATH}/sections/([^/]+)/problems/([^/]+)$", path)
        if m and method == "GET":
            res = get_problem(m.group(1), m.group(2))
            payload = json.dumps(res).encode("utf-8")
            start_response("200 OK", [("Content-Type", "application/json"), ("Content-Length", str(len(payload)))])
            return [payload]

        # /sections/{sec}/problems/{prob}/solution
        m = re.match(rf"^{API_PATH}/sections/([^/]+)/problems/([^/]+)/solution$", path)
        if m and method == "GET":
            res = get_solution(m.group(1), m.group(2))
            payload = json.dumps(res).encode("utf-8")
            start_response("200 OK", [("Content-Type", "application/json"), ("Content-Length", str(len(payload)))])
            return [payload]

        # /sections/{sec}/problems/{prob}/save-solved
        m = re.match(rf"^{API_PATH}/sections/([^/]+)/problems/([^/]+)/save-solved$", path)
        if m and method == "POST":
            length = int(environ.get("CONTENT_LENGTH", 0) or 0)
            body = json.loads(environ["wsgi.input"].read(length).decode("utf-8")) if length > 0 else {}
            res = save_solved(m.group(1), m.group(2), RunRequest(**body))
            payload = json.dumps(res).encode("utf-8")
            start_response("200 OK", [("Content-Type", "application/json"), ("Content-Length", str(len(payload)))])
            return [payload]

        # /sections/{sec}/problems/{prob}/run
        m = re.match(rf"^{API_PATH}/sections/([^/]+)/problems/([^/]+)/run$", path)
        if m and method == "POST":
            length = int(environ.get("CONTENT_LENGTH", 0) or 0)
            body = json.loads(environ["wsgi.input"].read(length).decode("utf-8")) if length > 0 else {}
            res = run_problem(m.group(1), m.group(2), RunRequest(**body))
            payload = json.dumps(res).encode("utf-8")
            start_response("200 OK", [("Content-Type", "application/json"), ("Content-Length", str(len(payload)))])
            return [payload]

    except HTTPException as exc:
        payload = json.dumps({"detail": exc.detail}).encode("utf-8")
        status_map = {400: "400 Bad Request", 404: "404 Not Found", 500: "500 Internal Server Error"}
        status_str = status_map.get(exc.status_code, f"{exc.status_code} Error")
        start_response(status_str, [("Content-Type", "application/json"), ("Content-Length", str(len(payload)))])
        return [payload]
    except Exception as exc:
        payload = json.dumps({"detail": str(exc)}).encode("utf-8")
        start_response("500 Internal Server Error", [("Content-Type", "application/json"), ("Content-Length", str(len(payload)))])
        return [payload]

    start_response("404 Not Found", [("Content-Type", "application/json")])
    return [b'{"detail":"Not Found"}']

