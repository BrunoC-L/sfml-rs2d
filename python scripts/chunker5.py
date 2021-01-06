from PIL import Image

image = Image.open('../assets/mapnoraidsdiv4v2.jpg')
pix = image.load()

def chunker(cx, cy, objects):
    wallsFile = open(f"../assets/walls/{cx}-{cy}-0.txt","r")
    outFile = open(f"../assets/chunks/{cx}-{cy}-0.txt","w+")
    for x in range(64):
        for y in range(64):
            colorIndex = 64 * y + x
            wall = wallsFile.readline()[:-1]
            objectsOnTile = objects.get((64 * cx + x, 64 * cy + y)) or ''
            outFile.write(f"{x};{y};{colorIndex};{wall};;;{objectsOnTile};;\n")
    outFile.close()
    wallsFile.close()
