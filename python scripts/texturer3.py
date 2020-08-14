from PIL import Image
from random import randint

dim = 32

def texturer(cx, cy):
    original = Image.open('../assets/mapnoraids.jpg')
    pixin = original.load()
    output = Image.new('RGB', (64 * dim, 64 * dim))
    pixout = output.load()
    for x in range(64):
        for y in range(64):
            for i in range(dim):
                for j in range(dim):
                    pixout[dim * x + i, dim * y + j] = pixin[(dim * (64 * cx + x) + i) // 8, (dim * (64 * cy + y) + j) // 8]
    output.save(f"../assets/textures/{cx}-{cy}-0.png")
    original.close()
