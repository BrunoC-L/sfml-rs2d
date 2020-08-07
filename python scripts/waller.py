from PIL import Image
image = Image.open('../assets/mapnoraids.jpg')
pix = image.load()

EAST  = 8
NORTH = 4
WEST  = 2
SOUTH = 1

diagonalsxy = [
    [
        (0, 0),
        (1, 1),
        (2, 2),
        (3, 3),
    ],
    [
        (0, 1),
        (1, 2),
        (2, 3),
    ],
    [
        (1, 0),
        (2, 1),
        (3, 2),
    ],
]

diagonalsnegxy = [
    [
        (0, 3),
        (1, 2),
        (2, 1),
        (3, 0),
    ],
    [
        (0, 2),
        (1, 1),
        (3, 0),
    ],
    [
        (1, 3),
        (2, 2),
        (3, 1),
    ],
]

northWalls = [
    [
        (0, 0),
        (1, 0),
        (2, 0),
        (3, 0),
    ],
    [
        (0, 1),
        (1, 1),
        (2, 1),
        (3, 1),
    ],
]

southWalls = [
    [
        (0, 2),
        (1, 2),
        (2, 2),
        (3, 2),
    ],
    [
        (0, 3),
        (1, 3),
        (2, 3),
        (3, 3),
    ],
]

westWalls = [
    [
        (0, 0),
        (0, 1),
        (0, 2),
        (0, 3),
    ],
    [
        (1, 0),
        (1, 1),
        (1, 2),
        (1, 3),
    ],
]

eastWalls = [
    [
        (2, 0),
        (2, 1),
        (2, 2),
        (2, 3),
    ],
    [
        (3, 0),
        (3, 1),
        (3, 2),
        (3, 3),
    ],
]

def hasWall(tile, setOfWalls):
    for possibleWall in setOfWalls:
        if [tile[4 * x + y] for x, y in possibleWall].count(True) >= 3:
            return True
    return False

def classify(x, y):
    tile = [False not in [c > 150 for c in pix[4 * x + i, 4 * y + j]] for i in range(4) for j in range(4)]
    if hasWall(tile, diagonalsnegxy):
        return 16
    elif hasWall(tile, diagonalsxy):
        return 17
    walls = 0
    if hasWall(tile,  eastWalls): walls |=  EAST
    if hasWall(tile,  westWalls): walls |=  WEST
    if hasWall(tile, southWalls): walls |= SOUTH
    if hasWall(tile, northWalls): walls |= NORTH
    return walls

for cx in range(29):
    for cy in range(25):
        print(25 * cx + cy + 1, ' out of ', 25 * 29)
        walls = [0] * 64 * 64
        for x in range(64):
            for y in range(64):
                walls[64* x + y] = classify(x + 64 * cx, y + 64 * cy)

        f = open(f"../assets/walls/{cx}-{cy}-0.txt","w+")
        for x in range(64):
            for y in range(64):
                w = walls[64 * x + y]
                if x >  0 and walls[64 * (x - 1) + y] &  EAST: w |=  WEST
                if y >  0 and walls[64 * x + (y - 1)] & SOUTH: w |= NORTH
                if x < 63 and walls[64 * (x + 1) + y] &  WEST: w |=  EAST
                if y < 63 and walls[64 * x + (y + 1)] & NORTH: w |= SOUTH
                f.write(f"{w}\n")
        f.close()
