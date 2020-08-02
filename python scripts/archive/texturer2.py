from PIL import Image
from random import randint

dim = 32
n = 17
m = int(256 / n)
color = ""
offset = 0

reference = open(f"mapcolors.txt","r")
image = Image.new('RGB', (1015 * dim, dim))
pix = image.load()

while(True):
    color = reference.readline()
    if not color:
        break
    color = int(color[:-1])
    r = n * (color // (n * n))
    color %= (n * n)
    g = n * (color // n)
    color %= n
    b = n * color
    for x in range(dim):
        for y in range(dim):
            pix[dim * offset + x, y] = (randint(r, r + m), randint(g, g + m), randint(b, b + m))
    offset += 1

reference.close()
image.save('../assets/test.png')
