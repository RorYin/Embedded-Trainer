import json

from fastapi.testclient import TestClient

import embedded_c_lab


client = TestClient(embedded_c_lab.app)


def test_custom_application_url_and_section_listing() -> None:
    page = client.get("/embedded-c-lab/")
    assert page.status_code == 200
    assert "Embedded C Lab" in page.text

    response = client.get("/embedded-c-lab/api/v1/sections")
    assert response.status_code == 200
    assert response.json()["sections"][0]["id"] == "basics"


def test_example_problem_and_path_traversal_rejection() -> None:
    response = client.get("/embedded-c-lab/api/v1/sections/basics/problems/hello-world")
    assert response.status_code == 200
    assert response.json()["title"] == "Hello, world!"

    traversal = client.get("/embedded-c-lab/api/v1/sections/../problems/secret")
    assert traversal.status_code == 404


def test_output_comparison_ignores_trailing_whitespace() -> None:
    assert embedded_c_lab.normalize_output("Hello\n\n") == embedded_c_lab.normalize_output("Hello")


def test_missing_compiler_is_reported(monkeypatch) -> None:
    monkeypatch.setattr(embedded_c_lab, "compiler_path", lambda: None)
    response = client.post(
        "/embedded-c-lab/api/v1/sections/basics/problems/hello-world/run",
        json={"source": "int main(void) { return 0; }"},
    )
    assert response.status_code == 503
    assert "GCC was not found" in response.json()["detail"]


def test_compile_saves_solution_and_progress_after_all_cases_pass(monkeypatch, tmp_path) -> None:
    lessons = tmp_path / "lessons"
    exercise = lessons / "basics" / "hello-world"
    exercise.mkdir(parents=True)
    (exercise / "problem.c").write_text("int main(void) { return 0; }\n", encoding="utf-8")
    (exercise / "problem_description.json").write_text(
        json.dumps({"title": "Hello", "description": "Print a greeting.", "hint": ""}),
        encoding="utf-8",
    )
    (exercise / "input_and_output.json").write_text(
        json.dumps({"cases": [{"input": "", "output": "Hello, Embedded C!\n"}]}),
        encoding="utf-8",
    )
    monkeypatch.setattr(embedded_c_lab, "lessons_root", lambda: lessons)
    assert embedded_c_lab.compiler_path() is not None
    source = '#include <stdio.h>\nint main(void) { puts("Hello, Embedded C!"); return 0; }'
    failed = client.post(
        "/embedded-c-lab/api/v1/sections/basics/problems/hello-world/run",
        json={"source": "int main(void) { return 0; }"},
    )
    assert failed.status_code == 200
    assert failed.json()["status"] == "failed"
    assert not (exercise / "problem_solved.c").exists()

    response = client.post(
        "/embedded-c-lab/api/v1/sections/basics/problems/hello-world/run",
        json={"source": source},
    )
    assert response.status_code == 200
    assert response.json()["status"] == "passed"
    assert response.json()["cases"][0]["actual"] == "Hello, Embedded C!\n"
    assert (exercise / "problem_solved.c").read_text(encoding="utf-8") == source
    assert client.get("/embedded-c-lab/api/v1/progress").json()["completed"] == {
        "basics/hello-world": True
    }
    saved_problem = client.get("/embedded-c-lab/api/v1/sections/basics/problems/hello-world")
    assert saved_problem.json()["completed"] is True
    assert saved_problem.json()["solved_code"] == source
    assert client.post(
        "/embedded-c-lab/api/v1/sections/basics/problems/hello-world/run",
        json={"source": source + "\n/* updated solution */"},
    ).json()["status"] == "passed"
    assert (exercise / "problem_solved.c").read_text(encoding="utf-8") == source + "\n/* updated solution */"


def test_multiple_test_cases_are_returned_individually(monkeypatch, tmp_path) -> None:
    lessons = tmp_path / "lessons"
    exercise = lessons / "basics" / "sum-two"
    exercise.mkdir(parents=True)
    (exercise / "input_and_output.json").write_text(
        json.dumps(
            {
                "cases": [
                    {"input": "2 3\n", "output": "5\n"},
                    {"input": "10 -4\n", "output": "6\n"},
                ]
            }
        ),
        encoding="utf-8",
    )
    monkeypatch.setattr(embedded_c_lab, "lessons_root", lambda: lessons)
    gcc = embedded_c_lab.compiler_path()
    if gcc is None:
        return
    monkeypatch.setattr(embedded_c_lab, "compiler_path", lambda: gcc)

    response = client.post(
        "/embedded-c-lab/api/v1/sections/basics/problems/sum-two/run",
        json={
            "source": (
                "#include <stdio.h>\n"
                "int main(void) { int a, b; "
                "if (scanf(\"%d %d\", &a, &b) != 2) return 1; "
                "printf(\"%d\\n\", a + b); return 0; }\n"
            )
        },
    )

    assert response.status_code == 200
    result = response.json()
    assert result["status"] == "passed"
    assert [case["input"] for case in result["cases"]] == ["2 3\n", "10 -4\n"]
    assert [case["expected"] for case in result["cases"]] == ["5\n", "6\n"]
    assert [case["actual"] for case in result["cases"]] == ["5\n", "6\n"]


def test_progress_is_saved_inside_lessons(monkeypatch, tmp_path) -> None:
    lessons = tmp_path / "lessons"
    exercise = lessons / "basics" / "hello-world"
    exercise.mkdir(parents=True)
    monkeypatch.setattr(embedded_c_lab, "application_root", lambda: tmp_path)
    monkeypatch.setattr(embedded_c_lab, "lessons_root", lambda: lessons)

    response = client.post(
        "/embedded-c-lab/api/v1/progress",
        json={"section_id": "basics", "problem_id": "hello-world", "passed": True},
    )

    assert response.status_code == 200
    assert (lessons / "progress.json").exists()
    assert not (tmp_path / "data" / "progress.json").exists()
    assert client.get("/embedded-c-lab/api/v1/progress").json() == {
        "completed": {"basics/hello-world": True}
    }

    client.post(
        "/embedded-c-lab/api/v1/progress",
        json={"section_id": "basics", "problem_id": "hello-world", "passed": False},
    )
    assert client.get("/embedded-c-lab/api/v1/progress").json() == {
        "completed": {"basics/hello-world": True}
    }
