from PIL import Image

image = Image.open('../assets/mapnoraidsdiv4.jpg')
pix = image.load()

n = 10
m = 26

for cx in range(29):
    for cy in range(25):
        print(25 * cx + cy + 1, ' out of ', 25 * 29)
        colors = [0] * (m + 1) * (m + 1) * (m + 1)
        for x in range(64):
            for y in range(64):
                r, g, b = pix[x + 64 * cx, y + 64 * cy]
                color = int(m * m * ((r + n/2) // n) + m * ((g + n/2) // n) + (b + n/2) // n)
                colors[color] += 1

        f = open(f"mapcolors/{cx}-{cy}-0.txt","w+")
        for i,count in enumerate(colors):
            if count > 0:
                f.write(f'{i}\n')
        f.close()