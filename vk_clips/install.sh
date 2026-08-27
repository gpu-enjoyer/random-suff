#!/usr/bin/env bash
set -euo pipefail

mkdir -p cache
mkdir -p cllips

python3 -m venv .venv

.venv/bin/pip install playwright requests websocket-client aiohttp aiofiles tqdm
.venv/bin/playwright install --with-deps chromium
