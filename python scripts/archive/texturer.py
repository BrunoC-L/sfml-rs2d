from PIL import Image
from random import randint

n = 8
m = int(256 / n)
dim = 32
image = Image.new('RGB', (n * n * n * dim, dim))
pix = image.load()

for r in range(n):
    for g in range(n):
        print(n * (n * r + g), ' out of ', n * n * n)
        for b in range(n):
            for x in range(dim):
                for y in range(dim):
                    offset = n * n * r + n * g + b
                    pix[dim * offset + x, y] = (randint(m * r, m * (r + 1)), randint(m * g, m * (g + 1)), randint(m * b, m * (b + 1)))

image.save('../assets/test.jpg')