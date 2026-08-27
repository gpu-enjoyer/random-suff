#!/usr/bin/env python3
"""Дебаг: что видит Playwright на странице VK-клипа."""
import asyncio
from pathlib import Path
from playwright.async_api import async_playwright

URL = "https://vk.ru/clip-105015964_456241418"
PROFILE = "vk-profile"
OUTDIR = Path("debug_screenshots")
OUTDIR.mkdir(exist_ok=True)

async def main():
    async with async_playwright() as p:
        ctx = await p.chromium.launch_persistent_context(
            user_data_dir=PROFILE,
            headless=False,
            viewport={"width": 1280, "height": 900},
            args=["--disable-blink-features=AutomationControlled", "--no-sandbox"],
        )
        page = await ctx.new_page()
        await page.goto(URL, wait_until="domcontentloaded")
        await asyncio.sleep(3)
        await page.screenshot(path=OUTDIR / "01_loaded.png", full_page=True)
        print("[1] Страница загружена. Скриншот:", OUTDIR / "01_loaded.png")

        # Ищем кнопку меню
        buttons = await page.locator('button').all()
        print(f"[2] Всего кнопок на странице: {len(buttons)}")
        for i, btn in enumerate(buttons[:20]):
            text = await btn.inner_text()
            aria = await btn.get_attribute("aria-label") or ""
            cls = await btn.get_attribute("class") or ""
            print(f"   btn[{i}] aria='{aria}' text='{text[:40]}' class='{cls[:60]}'")

        # Пробуем кликнуть по aria-label="Действия"
        try:
            await page.locator('[aria-label="Действия"]').first.click(timeout=3000)
            print("[3] Кликнул по [aria-label='Действия']")
        except Exception as e:
            print(f"[3] Не нашёл [aria-label='Действия']: {e}")
            # Fallback: ищем по тексту
            try:
                await page.locator("button").filter(has_text="Скачать").first.click(timeout=3000)
                print("[3] Кликнул по кнопке с текстом 'Скачать'")
            except Exception as e2:
                print(f"[3] Не нашёл кнопку 'Скачать': {e2}")

        await asyncio.sleep(2)
        await page.screenshot(path=OUTDIR / "02_after_click.png")
        print("[4] Скриншот после клика:", OUTDIR / "02_after_click.png")

        # Выводим HTML overlay/menu
        menus = await page.locator('[role="menu"], [class*="menu"], [class*="dropdown"], [class*="ActionSheet"]').all()
        print(f"[5] Найдено меню/оверлеев: {len(menus)}")
        for i, m in enumerate(menus):
            html = await m.inner_html()
            print(f"   menu[{i}] HTML preview: {html[:300]}...")

        await asyncio.sleep(10)
        await ctx.close()

asyncio.run(main())
