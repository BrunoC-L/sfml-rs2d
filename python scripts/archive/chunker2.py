from PIL import Image

# 69 106 11 should be green -> 128 + 24 + 0 = 152 -> 32 * 152 = 4864

def classify(r, g, b, a = 0):
    return 64 * (r // 32) + 8 * (g // 32) + (b // 32)

image = Image.open('../assets/mapnoraidsdiv4.jpg')
pix = image.load()

for cx in range(29):
    for cy in range(25):
        print(25 * cx + cy, ' out of ', 25 * 29)
        f = open(f"../assets/chunks/{cx}-{cy}-0.txt","w+")
        for x in range(64):
            for y in range(64):
                f.write(f"{x};{y};{classify(*pix[x + 64 * cx, y + 64 * cy])};15;;;;;\n")
        f.close()