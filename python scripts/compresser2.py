from PIL import Image

def compress(cx, cy, n ,m):
    image = Image.open('../assets/mapnoraidsdiv4v2.jpg')
    pix = image.load()
    colors = [0] * (m + 1) * (m + 1) * (m + 1)
    for x in range(64):
        for y in range(64):
            r, g, b = pix[x + 64 * cx, y + 64 * cy]
            color = int(m * m * ((r + n/2 - 1) // n) + m * ((g + n/2 - 1) // n) + (b + n/2 - 1) // n)
            colors[color] += 1
    f = open(f"mapcolors/{cx}-{cy}-0.txt","w+")
    for i,count in enumerate(colors):
        if count > 0:
            f.write(f'{i}\n')
    f.close()