#!/usr/bin/env bash
set -euo pipefail

[ ! -d .venv ] || [ ! -d cache ] && ./install.sh

.venv/bin/python src/get_html.py
.venv/bin/python src/get_links.py

# head -2 cache/links.txt > cache/links_head.txt
# .venv/bin/python src/download.py cache/links_head.txt
