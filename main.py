import threading
import webbrowser

import uvicorn

from embedded_c_lab import BASE_PATH, HOST, PORT, app


def main() -> None:
    url = f"http://{HOST}:{PORT}{BASE_PATH}/"
    threading.Timer(1.0, webbrowser.open, args=(url,)).start()
    uvicorn.run(app, host=HOST, port=PORT, log_level="info")


if __name__ == "__main__":
    main()
