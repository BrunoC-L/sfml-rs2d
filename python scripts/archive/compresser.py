from PIL import Image

image = Image.open('../assets/mapnoraidsdiv4.jpg')
pix = image.load()
colors = [0] * 18 * 18 * 18

for cx in range(29):
    for cy in range(25):
        print(25 * cx + cy + 1, ' out of ', 25 * 29)
        for x in range(64):
            for y in range(64):
                r, g, b = pix[x + 64 * cx, y + 64 * cy]
                colors[17 * 17 * ((r + 8) // 17) + 17 * ((g + 8) // 17) + (b + 8) // 17] += 1

f = open("mapcolors.txt","w+")
for i,count in enumerate(colors):
    if count > 0:
        x += 1
        f.write(f'{i}\n')
print(x)
f.close()