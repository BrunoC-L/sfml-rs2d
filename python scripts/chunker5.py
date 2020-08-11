from PIL import Image

image = Image.open('../assets/mapnoraidsdiv4v2.jpg')
pix = image.load()

def chunker(cx, cy):
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
    n = 10
    m = 26
    def classify(r, g, b, a = 0):
        color = m * m * ((r + n/2 - 1) // n) + m * ((g + n/2 - 1) // n) + (b + n/2 - 1) // n
        index = reverseColor[int(color)]
        return index
    
    wallsFile = open(f"../assets/walls/{cx}-{cy}-0.txt","r")
    objectsFile = open(f"../assets/objects/{cx}-{cy}-0.txt","r")
    outFile = open(f"../assets/chunks/{cx}-{cy}-0.txt","w+")
    
    for x in range(64):
        for y in range(64):
            colorIndex = classify(*pix[x + 64 * cx, y + 64 * cy])
            wall = wallsFile.readline()[:-1]
            objects = objectsFile.readline()[:-1]
            outFile.write(f"{x};{y};{colorIndex};{wall};{objects};;;;\n")
    outFile.close()
    wallsFile.close()
