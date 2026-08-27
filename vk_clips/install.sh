#!/usr/bin/env bash
set -euo pipefail

mkdir -p cache
mkdir -p clips

python3 -m venv .venv

.venv/bin/pip install playwright requests websocket-client
.venv/bin/pip install -U yt-dlp
.venv/bin/playwright install --with-deps chromium
