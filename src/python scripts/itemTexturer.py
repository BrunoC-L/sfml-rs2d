import os
from PIL import Image
from random import randint

def itemsTexturer():
    # files = os.listdir(directory)
    files = [
        'Logs.png',
        'Copper ore.png',
    ]
    dim = 128
    sizeX = 32768
    sizeY = dim * (len(files) // (sizeX // dim) + 1)
    count = 0
    directory = '../assets/items/'

    output = Image.new('RGB', (sizeX, sizeY))
    output.putalpha(255)

    for filename in files:
        if not filename.endswith(".png"): 
            print(os.path.join(directory, filename))
            continue
        inputFile = Image.open(os.path.join(directory, filename))
        x = count % dim
        y = count // dim
        output.paste(inputFile, (dim * x, dim * y))
        count += 1
        print(f'{count} / {len(files)}')
                
    output.save(f"../assets/textures/items.png")

if __name__ == '__main__':
    itemsTexturer()
