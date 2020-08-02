from PIL import Image

reference = open(f"mapcolors.txt","r")
color = ""
index = 0
reverseColor = [0] * 18 * 18 * 18
while(True):
    color = reference.readline()
    if not color:
        break
    index += 1
    reverseColor[int(color[:-1])] = index

reference.close()

def classify(r, g, b, a = 0):
    index = reverseColor[17 * 17 * ((r + 8) // 17) + 17 * ((g + 8) // 17) + ((b + 8) // 17)]
    return index if index < 1024 else 1023

image = Image.open('../assets/mapnoraidsdiv4.jpg')
pix = image.load()

for cx in range(29):
    for cy in range(25):
        print(25 * cx + cy + 1, ' out of ', 25 * 29)
        f = open(f"../assets/chunks/{cx}-{cy}-0.txt","w+")
        for x in range(64):
            for y in range(64):
                f.write(f"{x};{y};{classify(*pix[x + 64 * cx, y + 64 * cy])};15;;;;;\n")
        f.close()