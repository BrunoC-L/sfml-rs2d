from PIL import Image

image = Image.open('../assets/mapnoraidsdiv4v2.jpg')
pix = image.load()
for cx in range(29):
    for cy in range(25):
        print(25 * cx + cy + 1, ' out of ', 25 * 29)
        colorFile = open(f"mapcolors/{cx}-{cy}-0.txt","r")
        color = ""
        index = 0
        reverseColor = [0] * 27 * 27 * 27
        while(True):
            color = colorFile.readline()
            if not color:
                break
            reverseColor[int(color[:-1])] = index
            index += 1
        colorFile.close()

        def classify(r, g, b, a = 0):
            color = 26 * 26 * ((r + 5) // 10) + 26 * ((g + 5) // 10) + (b + 5) // 10
            index = reverseColor[color]
            return index

        f = open(f"../assets/chunks/{cx}-{cy}-0.txt","w+")
        for x in range(64):
            for y in range(64):
                colorIndex = classify(*pix[x + 64 * cx, y + 64 * cy])
                f.write(f"{x};{y};{colorIndex};0;;;;;\n")
        f.close()