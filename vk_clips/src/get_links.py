
import re

html = open("cache/bookmarks.html", encoding='utf-8').read()
links = re.findall(r'href="(/clip-?\d+_\d+)[^"]*"', html)
unique = sorted(set('https://vk.ru' + l for l in links))

open('cache/links.txt', 'w', encoding='utf-8').write('\n'.join(unique))
print(f'Найдено клипов: {len(unique)}')
