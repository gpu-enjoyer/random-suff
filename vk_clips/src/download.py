
#!/usr/bin/env python3
"""
VK Clips Downloader via Playwright (persistent profile)
=========================================================
Использует существующий профиль Chromium (./vk-profile) с живой сессией VK.

Запуск:
    python vk_clip_downloader.py --urls clips.txt --output clips/

Требования:
    pip install playwright aiohttp aiofiles tqdm
    playwright install chromium

Архитектура:
    - launch_persistent_context(user_data_dir="vk-profile") — ваша сессия VK.
    - Переиспользуется одна страница (быстрее new_page() × 1533).
    - Клик ⋯ → «Скачать» + перехват network response / download event.
    - Прямая ссылка качается через aiohttp (не через браузер).
    - Прогресс, retry, rate-limiting, обработка капчи.
"""

import asyncio
import argparse
import re
import sys
from pathlib import Path
from datetime import datetime
from typing import Optional, Set
from dataclasses import dataclass

from playwright.async_api import async_playwright, Page, Response, Download
import aiohttp
import aiofiles
from tqdm import tqdm


@dataclass
class Config:
    urls_file: Path
    output_dir: Path
    downloaded_log: Path
    failed_log: Path
    profile_dir: Path
    max_retries: int = 3
    delay: float = 3.0
    page_timeout: int = 30000
    headless: bool = False


class VKDownloader:
    MENU_BUTTON_SELECTORS = [
        '[aria-label="Действия"]',
        '[aria-label="More"]',
        'button[role="button"]:has(.vkuiIcon--more_vertical)',
        'button:has(svg)',
    ]

    def __init__(self, cfg: Config):
        self.cfg = cfg
        self.cfg.output_dir.mkdir(parents=True, exist_ok=True)
        self.downloaded: Set[str] = self._load_lines(cfg.downloaded_log)
        self.failed: Set[str] = self._load_lines(cfg.failed_log)
        self.session: Optional[aiohttp.ClientSession] = None

    @staticmethod
    def _load_lines(path: Path) -> Set[str]:
        if not path.exists():
            return set()
        return {line.strip().split()[0] for line in path.read_text().splitlines() if line.strip()}

    async def _append_line(self, path: Path, text: str):
        async with aiofiles.open(path, "a") as f:
            await f.write(f"{text}\\n")

    def _clip_id_from_url(self, url: str) -> str:
        return url.rstrip("/").split("/")[-1]

    async def _download_direct(self, url: str, filename: str) -> bool:
        filepath = self.cfg.output_dir / filename
        if filepath.exists():
            return True

        headers = {
            "User-Agent": (
                "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 "
                "(KHTML, like Gecko) Chrome/128.0.0.0 Safari/537.36"
            ),
            "Referer": "https://vk.ru/",
            "Accept": "*/*",
        }

        try:
            async with self.session.get(url, headers=headers, timeout=aiohttp.ClientTimeout(total=120)) as resp:
                resp.raise_for_status()
                async with aiofiles.open(filepath, "wb") as f:
                    async for chunk in resp.content.iter_chunked(65536):
                        await f.write(chunk)
            return True
        except Exception as e:
            tqdm.write(f"  [aiohttp error] {e}")
            return False

    async def _extract_via_network(self, page: Page, url: str) -> Optional[str]:
        direct_url: Optional[str] = None
        download_event: Optional[Download] = None

        async def on_response(response: Response):
            nonlocal direct_url
            try:
                r_url = response.url
                if "al_video.php" in r_url:
                    body = await response.text()
                    matches = re.findall(r'"url":"(https://[^"]+\\.mp4[^"]*)"', body)
                    if matches:
                        direct_url = matches[0].replace("\\\\/", "/")
                elif ".mp4" in r_url and ("vkvd" in r_url or "mycdn" in r_url or "okcdn" in r_url):
                    direct_url = r_url
            except Exception:
                pass

        page.on("response", on_response)

        try:
            await page.goto(url, wait_until="domcontentloaded", timeout=self.cfg.page_timeout)
            await asyncio.sleep(1.5)

            # Капча / блокировка
            if await page.locator('text=Подтвердите, что вы не робот').is_visible(timeout=2000):
                tqdm.write(f"  [!] Капча на {url}. Решите вручную и нажмите Enter в терминале...")
                input()
                await page.reload(wait_until="domcontentloaded")
                await asyncio.sleep(1.5)

            # --- Шаг 1: открыть меню (три точки) ---
            menu_opened = False
            for sel in self.MENU_BUTTON_SELECTORS:
                try:
                    loc = page.locator(sel).first
                    if await loc.is_visible(timeout=1500):
                        await loc.click()
                        menu_opened = True
                        break
                except Exception:
                    continue

            if not menu_opened:
                try:
                    await page.locator("button").filter(has_text=re.compile(r"[⋮⋯…]")).first.click(timeout=2000)
                    menu_opened = True
                except Exception:
                    pass

            if not menu_opened:
                return None

            await asyncio.sleep(0.5)

            # --- Шаг 2: кликнуть «Скачать» ---
            try:
                async with page.expect_download(timeout=10000) as dl_info:
                    await page.get_by_text("Скачать", exact=True).click(timeout=5000)
                download_event = await dl_info.value
            except Exception:
                try:
                    async with page.expect_download(timeout=10000) as dl_info:
                        await page.locator("text=Скачать").first.click(timeout=3000)
                    download_event = await dl_info.value
                except Exception:
                    pass

            for _ in range(20):
                if direct_url or download_event:
                    break
                await asyncio.sleep(0.5)

            if download_event and not direct_url:
                d_url = download_event.url
                if d_url.startswith("http"):
                    direct_url = d_url
                else:
                    await download_event.cancel()

            return direct_url

        except Exception as e:
            tqdm.write(f"  [extract error] {e}")
            return None
        finally:
            page.remove_listener("response", on_response)

    async def _process_single(self, page: Page, url: str, pbar: tqdm) -> bool:
        if url in self.downloaded:
            pbar.update(1)
            return True

        clip_id = self._clip_id_from_url(url)
        filename = f"{clip_id}.mp4"

        for attempt in range(1, self.cfg.max_retries + 1):
            direct_url = await self._extract_via_network(page, url)

            if direct_url:
                ok = await self._download_direct(direct_url, filename)
                if ok:
                    await self._append_line(self.cfg.downloaded_log, url)
                    self.downloaded.add(url)
                    pbar.update(1)
                    return True
                else:
                    tqdm.write(f"  [retry {attempt}/{self.cfg.max_retries}] download failed for {clip_id}")
            else:
                tqdm.write(f"  [retry {attempt}/{self.cfg.max_retries}] no URL extracted for {clip_id}")

            await asyncio.sleep(self.cfg.delay * attempt)

        await self._append_line(self.cfg.failed_log, f"{url}\\tno_url_or_download_failed\\t{datetime.now().isoformat()}")
        self.failed.add(url)
        pbar.update(1)
        return False

    async def run(self):
        raw_urls = [line.strip() for line in self.cfg.urls_file.read_text().splitlines() if line.strip()]
        urls = [u for u in raw_urls if u not in self.downloaded]

        if not urls:
            print("Все URL уже скачаны.")
            return

        print(f"К скачиванию: {len(urls)} / {len(raw_urls)} клипов")
        print(f"Профиль Chromium: {self.cfg.profile_dir}")
        print(f"Задержка: {self.cfg.delay}s | Попыток: {self.cfg.max_retries}")
        print()

        connector = aiohttp.TCPConnector(limit=10, limit_per_host=5)
        timeout = aiohttp.ClientTimeout(total=300)
        self.session = aiohttp.ClientSession(connector=connector, timeout=timeout)

        try:
            async with async_playwright() as p:
                context = await p.chromium.launch_persistent_context(
                    user_data_dir=str(self.cfg.profile_dir),
                    headless=self.cfg.headless,
                    accept_downloads=True,
                    viewport={"width": 1280, "height": 900},
                    locale="ru-RU",
                    timezone_id="Europe/Moscow",
                    args=[
                        "--disable-blink-features=AutomationControlled",
                        "--no-sandbox",
                        "--disable-dev-shm-usage",
                    ],
                )
                page = await context.new_page()

                with tqdm(total=len(urls), desc="Клипы", unit="clip") as pbar:
                    for url in urls:
                        await self._process_single(page, url, pbar)
                        await asyncio.sleep(self.cfg.delay)

                await context.close()
        finally:
            await self.session.close()

        print(f"\\nГотово. Скачано: {len(self.downloaded)}, Ошибок: {len(self.failed)}")


def main():
    parser = argparse.ArgumentParser(
        description="Скачивание VK-клипов через Playwright (persistent profile)"
    )
    parser.add_argument("--urls", default="clips.txt", help="Файл со ссылками")
    parser.add_argument("--output", default="clips", help="Папка для сохранения MP4")
    parser.add_argument("--profile", default="vk-profile", help="Директория persistent profile Chromium")
    parser.add_argument("--downloaded-log", default="downloaded.txt", help="Лог уже скачанных")
    parser.add_argument("--failed-log", default="failed.txt", help="Лог ошибок")
    parser.add_argument("--delay", type=float, default=3.0, help="Задержка между клипами, сек")
    parser.add_argument("--retries", type=int, default=3, help="Попыток на клип")
    parser.add_argument("--headless", action="store_true", help="Без GUI (может не работать с VK)")

    args = parser.parse_args()

    cfg = Config(
        urls_file=Path(args.urls),
        output_dir=Path(args.output),
        downloaded_log=Path(args.downloaded_log),
        failed_log=Path(args.failed_log),
        profile_dir=Path(args.profile),
        delay=args.delay,
        max_retries=args.retries,
        headless=args.headless,
    )

    asyncio.run(VKDownloader(cfg).run())


if __name__ == "__main__":
    main()

