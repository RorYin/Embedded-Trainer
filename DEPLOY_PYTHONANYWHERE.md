# Deploying Embedded C Lab to PythonAnywhere

This guide provides step-by-step instructions to deploy **Embedded C Lab** as a live web application on [PythonAnywhere](https://www.pythonanywhere.com/).

Because Embedded C Lab uses client-side **WebAssembly (`c2wasm`)** for all C compilation and execution, the backend server requires **no GCC compiler**, uses minimal CPU/memory, and runs effortlessly on PythonAnywhere's free tier.

---

## Prerequisites

1. A [PythonAnywhere](https://www.pythonanywhere.com/) account (Free "Beginner" tier or paid).
2. Your PythonAnywhere username (referred to as `<username>` in this guide).

---

## Step 1: Clone Repository in PythonAnywhere Console

1. Log in to your PythonAnywhere dashboard.
2. Go to the **Consoles** tab and click **Bash** to open a new terminal.
3. Clone your GitHub repository into your home directory:

```bash
git clone https://github.com/RorYin/Embedded-Trainer.git
cd Embedded-Trainer
```

---

## Step 2: Set Up Virtual Environment & Dependencies

Inside the Bash console, create a virtual environment using Python 3.11 and install dependencies:

```bash
# 1. Create a virtual environment with Python 3.11
python3.11 -m venv .venv

# 2. Activate the virtual environment
source .venv/bin/activate

# 3. Upgrade pip and install project requirements
pip install --upgrade pip
pip install -r requirements.txt

# 4. Install a2wsgi (WSGI adapter required for ASGI / FastAPI on PythonAnywhere)
pip install a2wsgi

# 5. (Optional) Download Monaco Editor assets for offline / fast local caching
python scripts/download_monaco.py
```

---

## Step 3: Create the Web App on PythonAnywhere

1. Go to the **Web** tab on the PythonAnywhere dashboard.
2. Click **Add a new web app**.
3. When prompted about your domain name, click **Next** (using your default `<username>.pythonanywhere.com`).
4. Select **Manual configuration** (do **NOT** select Django or Flask).
5. Choose **Python 3.11**.
6. Click **Next** to finalize creation.

---

## Step 4: Configure Virtualenv & Paths

On your web app configuration page (under the **Web** tab):

### 1. Code Section
- **Source code**: `/home/<username>/Embedded-Trainer`
- **Working directory**: `/home/<username>/Embedded-Trainer`

*(Replace `<username>` with your actual PythonAnywhere username)*

### 2. Virtualenv Section
- Click the path edit box under **Virtualenv** and enter:
  ```
  /home/<username>/Embedded-Trainer/.venv
  ```
- Click the blue checkmark to save.

---

## Step 5: Configure the WSGI File

PythonAnywhere routes web traffic through a WSGI configuration file. Because FastAPI is an ASGI framework, we use `a2wsgi` to bridge requests seamlessly.

1. Under the **Code** section on the **Web** tab, click on the **WSGI configuration file** link (e.g. `/var/www/<username>_pythonanywhere_com_wsgi.py`).
2. Delete everything currently inside that file.
3. Paste the following configuration:

```python
import sys
import os
from a2wsgi import ASGIMiddleware

# Define project home directory
project_home = '/home/<username>/Embedded-Trainer'
if project_home not in sys.path:
    sys.path.insert(0, project_home)

# Ensure current working directory is the project directory (for lessons/ path lookup)
os.chdir(project_home)

# Import the FastAPI application
from embedded_c_lab import app

# Wrap FastAPI ASGI app with WSGI middleware for PythonAnywhere
application = ASGIMiddleware(app)
```

*(Remember to replace `<username>` with your actual PythonAnywhere username!)*

4. Click **Save** in the top right corner.

---

## Step 6: Static Files (Optional Performance Optimization)

To serve static assets (CSS, JS, WebAssembly binaries) directly via Nginx instead of Python:

1. Scroll down to the **Static files** section on the **Web** tab.
2. Add an entry:
   - **URL**: `/embedded-c-lab/static/`
   - **Directory**: `/home/<username>/Embedded-Trainer/static/`
3. Click the checkmark to save.

---

## Step 7: Reload and Test Your Web App

1. Scroll to the top of the **Web** tab.
2. Click the green **Reload <username>.pythonanywhere.com** button.
3. Open your web app at:
   ```
   https://<username>.pythonanywhere.com/
   ```
4. You will be redirected to `https://<username>.pythonanywhere.com/embedded-c-lab/`.
5. Select a problem (e.g., *Bit Manipulation -> GPIO Register Operations*), write code or click **Load Reference Solution**, and click **Run Code**. Everything will compile and run directly in your browser!

---

## Troubleshooting & Maintenance

### Viewing Server Logs
If you encounter a `500 Internal Server Error`:
- Check the **Error log**: Located on the **Web** tab under **Log files** (`/var/www/<username>_pythonanywhere_com_error.log`).
- Check the **Server log**: (`/var/www/<username>_pythonanywhere_com_server.log`).

### Updating Code from GitHub
Whenever you push updates to GitHub, pull the changes on PythonAnywhere:

```bash
cd ~/Embedded-Trainer
git pull origin main
```
Then go to the **Web** tab and click **Reload**.
