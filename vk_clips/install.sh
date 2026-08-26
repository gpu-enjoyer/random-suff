#!/usr/bin/env bash
set -euo pipefail

python3 -m venv .venv

mkdir cache

.venv/bin/pip install playwright requests websocket-client
.venv/bin/playwright install --with-deps chromium
