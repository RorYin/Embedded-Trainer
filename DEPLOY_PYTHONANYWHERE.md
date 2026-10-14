# Deploying Embedded C Lab to PythonAnywhere (Simple Direct Install)

This guide provides the simplest, easiest way to deploy **Embedded C Lab** to [PythonAnywhere](https://www.pythonanywhere.com/) without any virtual environment setup or version mismatch issues.

---

## Step 1: Open Bash Console & Clone Repository

1. Log in to your [PythonAnywhere Dashboard](https://www.pythonanywhere.com/).
2. Under the **Consoles** tab, click **Bash** (or click **"Open Bash console here"** in the Files tab).
3. Clone your GitHub repository:

```bash
git clone https://github.com/RorYin/Embedded-Trainer.git
cd Embedded-Trainer
```

---

## Step 2: Install Packages Directly (No Virtualenv)

Run this single command in the Bash console to install FastAPI, a2wsgi, and all dependencies directly into your PythonAnywhere user environment:

```bash
pip install --user -r requirements.txt
```

*(Optional: Run `python scripts/download_monaco.py` to cache Monaco editor assets locally).*

---

## Step 3: Configure the Web App

1. Go to the **Web** tab in your PythonAnywhere dashboard.
2. If you haven't created a web app yet:
   - Click **Add a new web app**.
   - Select your default domain (`<username>.pythonanywhere.com`).
   - Choose **Manual configuration** (do **not** select Flask or Django).
   - Select your Python version (e.g. **Python 3.10** or **Python 3.11**).
3. On the Web configuration page:
   - **Source code**: `/home/<username>/Embedded-Trainer`
   - **Working directory**: `/home/<username>/Embedded-Trainer`
   - **Virtualenv**: **LEAVE THIS EMPTY** (delete any path if present).

*(Replace `<username>` with your actual username, e.g. `clab`)*

---

## Step 4: Configure `wsgi.py`

1. Under the **Code** section on the **Web** tab, click the link to your **WSGI configuration file** (e.g. `/var/www/<username>_pythonanywhere_com_wsgi.py`).
2. Replace everything in that file with these simple lines:

```python
import sys
import os

# Project home directory
project_home = '/home/<username>/Embedded-Trainer'
if project_home not in sys.path:
    sys.path.insert(0, project_home)

os.chdir(project_home)

# Native synchronous WSGI handler (zero async deadlocks or uWSGI thread timeouts)
from embedded_c_lab import wsgi_app as application
```

*(Make sure to change `<username>` to your PythonAnywhere username, e.g. `clab`).*


3. Click **Save** in the top right.

---

## Step 5: (Optional) Static Files

Under **Static files** on the **Web** tab, add:
- **URL**: `/embedded-c-lab/static/`
- **Directory**: `/home/<username>/Embedded-Trainer/static/`

---

## Step 6: Reload & Launch!

1. Click the green **Reload <username>.pythonanywhere.com** button at the top of the Web tab.
2. Open your web app:
   ```
   https://<username>.pythonanywhere.com/
   ```
   *(It will automatically open `/embedded-c-lab/`)*.
