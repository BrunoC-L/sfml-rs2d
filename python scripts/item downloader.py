import re
import requests
from bs4 import BeautifulSoup

sites = [
    'https://oldschool.runescape.wiki/w/Category:Detailed_item_images?filefrom=#mw-category-media',
    'https://oldschool.runescape.wiki/w/Category:Detailed_item_images?filefrom=Agility+arena+ticket+detail.png#mw-category-media',
    'https://oldschool.runescape.wiki/w/Category:Detailed_item_images?filefrom=Antifire+potion%281%29+detail.png#mw-category-media',
    'https://oldschool.runescape.wiki/w/Category:Detailed_item_images?filefrom=Bagged+sunflower+detail.png#mw-category-media',
    'https://oldschool.runescape.wiki/w/Category:Detailed_item_images?filefrom=Bird+nest+detail.png#mw-category-media',
    'https://oldschool.runescape.wiki/w/Category:Detailed_item_images?filefrom=Blood%27n%27tar+snelm+%28pointed%29+detail.png#mw-category-media',
    'https://oldschool.runescape.wiki/w/Category:Detailed_item_images?filefrom=Box+trap+detail.png#mw-category-media',
    'https://oldschool.runescape.wiki/w/Category:Detailed_item_images?filefrom=Bucket+of+sand+detail.png#mw-category-media',
    'https://oldschool.runescape.wiki/w/Category:Detailed_item_images?filefrom=Certificate+%28Death+Plateau%29+detail.png#mw-category-media',
    'https://oldschool.runescape.wiki/w/Category:Detailed_item_images?filefrom=Comp+ogre+bow+detail.png#mw-category-media',
    'https://oldschool.runescape.wiki/w/Category:Detailed_item_images?filefrom=Crystal+legs+detail.png#mw-category-media',
    'https://oldschool.runescape.wiki/w/Category:Detailed_item_images?filefrom=Defence+hood+detail.png#mw-category-media',
    'https://oldschool.runescape.wiki/w/Category:Detailed_item_images?filefrom=Dragon+med+helm+detail.png#mw-category-media',
    'https://oldschool.runescape.wiki/w/Category:Detailed_item_images?filefrom=Elven+skirt+%28yellow%29+detail.png#mw-category-media',
    'https://oldschool.runescape.wiki/w/Category:Detailed_item_images?filefrom=Fenkenstrain%27s+castle+teleport+%28tablet%29+detail.png#mw-category-media',
    'https://oldschool.runescape.wiki/w/Category:Detailed_item_images?filefrom=Garlic+powder+detail.png#mw-category-media',
    'https://oldschool.runescape.wiki/w/Category:Detailed_item_images?filefrom=Graceful+boots+detail.png#mw-category-media',
    'https://oldschool.runescape.wiki/w/Category:Detailed_item_images?filefrom=Guam+potion+%28unf%29+detail.png#mw-category-media',
    'https://oldschool.runescape.wiki/w/Category:Detailed_item_images?filefrom=Hitpoints+hood+detail.png#mw-category-media',
    'https://oldschool.runescape.wiki/w/Category:Detailed_item_images?filefrom=Iron+key+%28H.A.M.%29+detail.png#mw-category-media',
    'https://oldschool.runescape.wiki/w/Category:Detailed_item_images?filefrom=Kharidian+headpiece+detail.png#mw-category-media',
    'https://oldschool.runescape.wiki/w/Category:Detailed_item_images?filefrom=Long+vine+detail.png#mw-category-media',
    'https://oldschool.runescape.wiki/w/Category:Detailed_item_images?filefrom=Marble+magic+wardrobe+%28flatpack%29+detail.png#mw-category-media',
    'https://oldschool.runescape.wiki/w/Category:Detailed_item_images?filefrom=Mixture+-+step+1%281%29+detail.png#mw-category-media',
    'https://oldschool.runescape.wiki/w/Category:Detailed_item_images?filefrom=Newspaper+detail.png#mw-category-media',
    'https://oldschool.runescape.wiki/w/Category:Detailed_item_images?filefrom=Ornamental+globe+%28flatpack%29+detail.png#mw-category-media',
    'https://oldschool.runescape.wiki/w/Category:Detailed_item_images?filefrom=Poisoned+egg+detail.png#mw-category-media',
    'https://oldschool.runescape.wiki/w/Category:Detailed_item_images?filefrom=Ramrod+detail.png#mw-category-media',
    'https://oldschool.runescape.wiki/w/Category:Detailed_item_images?filefrom=Redwood+tree+seed+detail.png#mw-category-media',
    'https://oldschool.runescape.wiki/w/Category:Detailed_item_images?filefrom=Rune+hasta+detail.png#mw-category-media',
    'https://oldschool.runescape.wiki/w/Category:Detailed_item_images?filefrom=Saw+detail.png#mw-category-media',
    'https://oldschool.runescape.wiki/w/Category:Detailed_item_images?filefrom=Silver+key+%28H.A.M.%29+detail.png#mw-category-media',
    'https://oldschool.runescape.wiki/w/Category:Detailed_item_images?filefrom=Spicy+minced+meat+detail.png#mw-category-media',
    'https://oldschool.runescape.wiki/w/Category:Detailed_item_images?filefrom=Steel+studs+detail.png#mw-category-media',
    'https://oldschool.runescape.wiki/w/Category:Detailed_item_images?filefrom=Tanzanite+fang+detail.png#mw-category-media',
    'https://oldschool.runescape.wiki/w/Category:Detailed_item_images?filefrom=Tomatoes%285%29+detail.png#mw-category-media',
    'https://oldschool.runescape.wiki/w/Category:Detailed_item_images?filefrom=Ultimate+ironman+helm+detail.png#mw-category-media',
    'https://oldschool.runescape.wiki/w/Category:Detailed_item_images?filefrom=Vorkath%27s+head+detail.png#mw-category-media',
    'https://oldschool.runescape.wiki/w/Category:Detailed_item_images?filefrom=Wise+old+man%27s+santa+hat+detail.png#mw-category-media',
    'https://oldschool.runescape.wiki/w/Category:Detailed_item_images?filefrom=Zombie+head+detail.png#mw-category-media',
]

for i, site in enumerate(sites):
    print(f'{i} out of {len(sites)}')
    response = requests.get(site)
    soup = BeautifulSoup(response.text, 'html.parser')
    img_tags = soup.find_all('img')
    urls = [img['src'] for img in img_tags]
    print(len(urls))
    for url in urls:
        filename = url.split('/')
        filename = [x for x in filename if '.png' in x or '.jpg' in x or '.svg' in x or '.gif' in x][0]
        filename = (''.join([l for l in filename.replace('_',' ') if l >= 'a' and l <= 'z' or l >= 'A' and l <= 'Z' or l == ' ' or l == '.']))[:-10] +'.png'
        with open(f'../assets/items/{filename}', 'wb') as f:
            response = requests.get(f'https://oldschool.runescape.wiki{url}')
            f.write(response.content)
