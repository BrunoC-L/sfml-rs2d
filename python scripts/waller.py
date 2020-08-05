from PIL import Image
image = Image.open('../assets/mapnoraids.jpg')
pix = image.load()

EAST  = 8
NORTH = 4
WEST  = 2
SOUTH = 1

def hasDiagonalWall(x, y):
    XYDiag = True
    negXYDiag = True
    for i in range(4):
        if not XYDiag and not negXYDiag:
            return False
        if XYDiag:
            r,g,b = pix[4 * x + i, 4 * y - i]
            XYDiag = r > 100 and g > 100 and b > 100
        if negXYDiag:
            r,g,b = pix[4 * x + i, 4 * y + i]
            negXYDiag = r > 100 and g > 100 and b > 100
    return XYDiag or negXYDiag

def horizontalWalls(x, y):
    walls = [True] * 4
    for i in range(4):
        if not walls.__contains__(True):
            return False
        for w in range(4):
            if walls[w]:
                r,g,b = pix[4 * x + i, 4 * y + w]
                walls[w] = r > 100 and g > 100 and b > 100
    return (1 if (walls[0] or walls[1]) else 0) + (4 if (walls[2] or walls[3]) else 0)

def verticalWalls(x, y):
    walls = [True] * 4
    for i in range(4):
        if not walls.__contains__(True):
            return False
        for w in range(4):
            if walls[w]:
                r,g,b = pix[4 * x + w, 4 * y + i]
                walls[w] = r > 100 and g > 100 and b > 100
    return (8 if (walls[0] or walls[1]) else 0) + (2 if (walls[2] or walls[3]) else 0)

def classify(x, y):
    if hasDiagonalWall(x, y):
        return 15
    h = horizontalWalls(x, y)
    v = verticalWalls(x, y)
    return h + v

for cx in [18]:#range(29):
    for cy in [13]:#range(25):
        print(25 * cx + cy + 1, ' out of ', 25 * 29)
        walls = [0] * 64 * 64
        for x in range(64):
            for y in range(64):
                walls[64* x + y] = classify(x + 64 * cx, y + 64 * cy)

        f = open(f"../assets/walls/{cx}-{cy}-0.txt","w+")
        for x in range(64):
            for y in range(64):
                w = walls[64 * x + y]
                if x >  0 and walls[64 * (x-1) + y] &  EAST: w |=  WEST
                if y >  0 and walls[64 * x + (y-1)] & SOUTH: w |= NORTH
                if x < 63 and walls[64 * (x+1) + y] &  WEST: w |=  EAST
                if y < 63 and walls[64 * x + (y+1)] & NORTH: w |= SOUTH
                f.write(f"{w}\n")
        f.close()
