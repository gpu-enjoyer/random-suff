#!/usr/bin/env bash
set -euo pipefail

[ ! -d .venv ] || [ ! -d cache ] && ./install.sh

.venv/bin/python src/get_html.py
.venv/bin/python src/get_links.py
.venv/bin/yt-dlp -a cache/links.txt -o "clips/%(id)s.%(ext)s" --merge-output-format mp4
