from PIL import Image
from random import randint

dim = 32

def texturer(cx, cy, n, m):
    reference = open(f"mapcolors/{cx}-{cy}-0.txt","r")
    image = Image.new('RGB', (1024 * dim, dim))
    pix = image.load()
    color = ""
    offset = 0
    while(True):
        color = reference.readline()
        if not color:
            break
        color = int(color[:-1])
        b = n * (color % m)
        g = n * ((color // m) % m)
        r = n * ((color // (m * m)) % m)
        for x in range(dim):
            for y in range(dim):
                color =  (randint(r, r + n), randint(g, g + n), randint(b, b + n))
                pix[dim * offset + x, y] = color
        offset += 1
    reference.close()
    image.save(f"../assets/textures/{cx}-{cy}-0.png")
