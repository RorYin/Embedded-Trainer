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
    sections = response.json()["sections"]
    assert len(sections) == 5
    assert sections[0]["id"] == "bit_manipulation"


def test_example_problem_and_path_traversal_rejection() -> None:
    response = client.get("/embedded-c-lab/api/v1/sections/bit_manipulation/problems/gpio-pin-operations")
    assert response.status_code == 200
    data = response.json()
    assert data["title"] == "GPIO Register Operations (Set, Clear, Toggle, Read)"
    assert "cases" in data
    assert len(data["cases"]) >= 1

    traversal = client.get("/embedded-c-lab/api/v1/sections/../problems/secret")
    assert traversal.status_code == 404


def test_output_comparison_ignores_trailing_whitespace() -> None:
    assert embedded_c_lab.normalize_output("Hello\n\n") == embedded_c_lab.normalize_output("Hello")
    assert embedded_c_lab.normalize_output("5\r\n") == embedded_c_lab.normalize_output("5\n")


def test_no_local_compiler_required() -> None:
    # Verify that the backend has no local gcc path requirement
    assert not hasattr(embedded_c_lab, "compiler_path")


def test_save_solved_persists_solution_and_progress(monkeypatch, tmp_path) -> None:
    lessons = tmp_path / "lessons"
    exercise = lessons / "bit_manipulation" / "gpio-pin-operations"
    exercise.mkdir(parents=True)
    (exercise / "gpio_pin_operations.c").write_text("int main(void) { return 0; }\n", encoding="utf-8")
    (exercise / "problem_description.json").write_text(
        json.dumps({"title": "GPIO", "description": "Operations.", "hint": ""}),
        encoding="utf-8",
    )
    (exercise / "input_and_output.json").write_text(
        json.dumps({"cases": [{"input": "", "output": "All tests passed!\n"}]}),
        encoding="utf-8",
    )
    monkeypatch.setattr(embedded_c_lab, "lessons_root", lambda: lessons)

    source = '#include <stdio.h>\nint main(void) { printf("All tests passed!\\n"); return 0; }'
    save_resp = client.post(
        "/embedded-c-lab/api/v1/sections/bit_manipulation/problems/gpio-pin-operations/save-solved",
        json={"source": source, "save_file": True},
    )
    assert save_resp.status_code == 200
    assert save_resp.json()["saved"] is True
    assert (exercise / "gpio_pin_operations_solved.c").read_text(encoding="utf-8") == source
    assert client.get("/embedded-c-lab/api/v1/progress").json()["completed"] == {
        "bit_manipulation/gpio-pin-operations": True
    }
    saved_problem = client.get("/embedded-c-lab/api/v1/sections/bit_manipulation/problems/gpio-pin-operations")
    assert saved_problem.json()["completed"] is True
    assert saved_problem.json()["solved_code"] == source


def test_save_solved_default_does_not_create_file(monkeypatch, tmp_path) -> None:
    lessons = tmp_path / "lessons"
    exercise = lessons / "bit_manipulation" / "gpio-pin-operations"
    exercise.mkdir(parents=True)
    (exercise / "gpio_pin_operations.c").write_text("int main(void) { return 0; }\n", encoding="utf-8")
    (exercise / "problem_description.json").write_text(
        json.dumps({"title": "GPIO", "description": "Operations.", "hint": ""}),
        encoding="utf-8",
    )
    (exercise / "input_and_output.json").write_text(
        json.dumps({"cases": [{"input": "", "output": "All tests passed!\n"}]}),
        encoding="utf-8",
    )
    monkeypatch.setattr(embedded_c_lab, "lessons_root", lambda: lessons)

    source = '#include <stdio.h>\nint main(void) { printf("All tests passed!\\n"); return 0; }'
    save_resp = client.post(
        "/embedded-c-lab/api/v1/sections/bit_manipulation/problems/gpio-pin-operations/save-solved",
        json={"source": source},
    )
    assert save_resp.status_code == 200
    assert save_resp.json()["saved"] is False
    assert not (exercise / "gpio_pin_operations_solved.c").exists()
    assert client.get("/embedded-c-lab/api/v1/progress").json()["completed"] == {
        "bit_manipulation/gpio-pin-operations": True
    }


def test_run_endpoint_saves_solution(monkeypatch, tmp_path) -> None:
    lessons = tmp_path / "lessons"
    exercise = lessons / "bit_manipulation" / "gpio-pin-operations"
    exercise.mkdir(parents=True)
    (exercise / "gpio_pin_operations.c").write_text("int main(void) { return 0; }\n", encoding="utf-8")
    (exercise / "problem_description.json").write_text(
        json.dumps({"title": "GPIO", "description": "Operations.", "hint": ""}),
        encoding="utf-8",
    )
    (exercise / "input_and_output.json").write_text(
        json.dumps(
            {
                "cases": [
                    {"input": "", "output": "All tests passed!\n"},
                ]
            }
        ),
        encoding="utf-8",
    )
    monkeypatch.setattr(embedded_c_lab, "lessons_root", lambda: lessons)

    source = '#include <stdio.h>\nint main(void) { return 0; }'
    response = client.post(
        "/embedded-c-lab/api/v1/sections/bit_manipulation/problems/gpio-pin-operations/run",
        json={"source": source, "save_file": True},
    )
    assert response.status_code == 200
    result = response.json()
    assert result["passed"] is True
    assert result["saved"] is True
    assert (exercise / "gpio_pin_operations_solved.c").read_text(encoding="utf-8") == source


def test_progress_is_saved_inside_lessons(monkeypatch, tmp_path) -> None:
    lessons = tmp_path / "lessons"
    exercise = lessons / "bit_manipulation" / "gpio-pin-operations"
    exercise.mkdir(parents=True)
    monkeypatch.setattr(embedded_c_lab, "application_root", lambda: tmp_path)
    monkeypatch.setattr(embedded_c_lab, "lessons_root", lambda: lessons)

    response = client.post(
        "/embedded-c-lab/api/v1/progress",
        json={"section_id": "bit_manipulation", "problem_id": "gpio-pin-operations", "passed": True},
    )

    assert response.status_code == 200
    assert (lessons / "progress.json").exists()
    assert not (tmp_path / "data" / "progress.json").exists()
    assert client.get("/embedded-c-lab/api/v1/progress").json() == {
        "completed": {"bit_manipulation/gpio-pin-operations": True}
    }

    client.post(
        "/embedded-c-lab/api/v1/progress",
        json={"section_id": "bit_manipulation", "problem_id": "gpio-pin-operations", "passed": False},
    )
    assert client.get("/embedded-c-lab/api/v1/progress").json() == {
        "completed": {"bit_manipulation/gpio-pin-operations": True}
    }


def test_pointers_and_memory_section() -> None:
    # Verify section listing includes pointers_and_memory
    response = client.get("/embedded-c-lab/api/v1/sections")
    assert response.status_code == 200
    sections = {s["id"]: s for s in response.json()["sections"]}
    assert "pointers_and_memory" in sections
    assert "parse-uart-frame" in sections["pointers_and_memory"]["problems"]

    # Verify problem details
    problem_resp = client.get(
        "/embedded-c-lab/api/v1/sections/pointers_and_memory/problems/parse-uart-frame"
    )
    assert problem_resp.status_code == 200
    data = problem_resp.json()
    assert data["title"] == "Parsing UART Packet Frame using Pointer Arithmetic"
    assert data["starter_filename"] == "parse_uart_frame.c"
    assert data["solution_filename"] == "parse_uart_frame_solution.c"
    assert "parse_frame" in data["starter_code"]
    assert "parse_frame" in data["solution_code"]
    assert len(data["cases"]) >= 1


def test_solution_endpoint_returns_reference_code() -> None:
    resp = client.get("/embedded-c-lab/api/v1/sections/bit_manipulation/problems/gpio-pin-operations/solution")
    assert resp.status_code == 200
    data = resp.json()
    assert data["filename"] == "gpio_pin_operations_solution.c"
    assert "gpio_set_pin" in data["code"]


def test_save_solved_with_suitable_name(monkeypatch, tmp_path) -> None:
    lessons = tmp_path / "lessons"
    exercise = lessons / "bit_manipulation" / "my-test"
    exercise.mkdir(parents=True)
    (exercise / "my_test.c").write_text("int main(void) { return 0; }\n", encoding="utf-8")
    (exercise / "problem_description.json").write_text(
        json.dumps({"title": "My Test", "description": "Test.", "hint": ""}),
        encoding="utf-8",
    )
    (exercise / "input_and_output.json").write_text(
        json.dumps({"cases": [{"input": "", "output": "OK\n"}]}),
        encoding="utf-8",
    )
    monkeypatch.setattr(embedded_c_lab, "lessons_root", lambda: lessons)

    source = 'int main(void) { return 42; }'
    save_resp = client.post(
        "/embedded-c-lab/api/v1/sections/bit_manipulation/problems/my-test/save-solved",
        json={"source": source, "save_file": True},
    )
    assert save_resp.status_code == 200
    assert (exercise / "my_test_solved.c").read_text(encoding="utf-8") == source
