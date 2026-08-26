
from playwright.sync_api import sync_playwright

with sync_playwright() as p:
    context = p.chromium.launch_persistent_context('./vk-profile', headless=False)
    page = context.pages[0] if context.pages else context.new_page()
    page.goto('https://vk.ru/bookmarks',
        wait_until='domcontentloaded', timeout=60000)

    input('1/3 Войдите в профиль и нажмите Enter ...')

    prev, stable = -1, 0
    while stable < 10:
        page.evaluate('window.scrollTo(0, document.body.scrollHeight)')
        page.wait_for_timeout(1500)
        count = page.evaluate(
            "document.querySelectorAll('.bookmarks_row').length"
        )
        print(f'Загружено записей: {count}')
        if count == prev:
            stable += 1
        else:
            stable, prev = 0, count

    input('2/3 Скролл ленты завершен. '
          'Enter – сохранить html ...')

    open('cache/bookmarks.html', 'w', encoding='utf-8').write(page.content())

    input('3/3 Сохранено: cache/bookmarks.html '
          'Enter — закрыть браузер и выйти ...')
    context.close()
