from PIL import Image
from constants import NORTH, SOUTH, EAST, WEST, TILES, TPC
from scipy.stats import mode

'''
This script will open the world map and generate a file for each chunk's walls
Essentially we look at the image tile by tile and try to figure out if it has
a wall or not. There are more details in comments. There is 1 file per chunk.

TODO
There is a huge flaw with this script because there is no  difference between
a full wall and a fence currently, meaning you would not be able to use only
the resulting files from this script to generate either walls or fences in game
This can be fixed later with an editing tool possibly, where by highlighting certain
parts of the map you could indicate that walls in that section are in fact fences
One would have to generate 2 different sets of walls, 2 different files: https://i.imgur.com/gagCgsE.png
'''

image = Image.open('../assets/mapnoraids.jpg')
pix = image.load()

# Each tile on that map is 4x4 pixels, we scan each box of 16 pixels
# Looking for patterns of walls, for example 4 pixels alligned to the edge
# Or 4 pixels alligned but off by 1 pixel on that tile
# alot of walls are not exactly on the edge of the tile on the map...
# For diagonals we look for full diagonal (4 long) or 3 diagonal either 1 px above or 1px below
# For vertical/horizontal we look for full walls but allow the wall to be off by 1 px

diagonalsxy = [
    [ (0, 0), (1, 1), (2, 2), (3, 3) ],
    [ (0, 1), (1, 2), (2, 3) ],
    [ (1, 0), (2, 1), (3, 2) ],
]

diagonalsnegxy = [
    [ (0, 3), (1, 2), (2, 1), (3, 0) ],
    [ (0, 2), (1, 1), (3, 0) ],
    [ (1, 3), (2, 2), (3, 1) ],
]

northWalls = [
    [ (0, 0), (1, 0), (2, 0), (3, 0) ],
    [ (0, 1), (1, 1), (2, 1), (3, 1) ],
]

southWalls = [
    [ (0, 2), (1, 2), (2, 2), (3, 2) ],
    [ (0, 3), (1, 3), (2, 3), (3, 3) ],
]

westWalls = [
    [ (0, 0), (0, 1), (0, 2), (0, 3) ],
    [ (1, 0), (1, 1), (1, 2), (1, 3) ],
]

eastWalls = [
    [ (2, 0), (2, 1), (2, 2), (2, 3) ],
    [ (3, 0), (3, 1), (3, 2), (3, 3) ],
]

baseWaterColor = (98, 118, 168)
baseLavaColor  = (249, 205, 18)
kindaGraySnow  = (230, 230, 230)

def distance(color1, color2):
    return sum([abs(c1-c2) for c1, c2 in zip(color1, color2)]) 

def hasWall(tile, setOfWalls):
    for possibleWall in setOfWalls:
        # Note the - 1, we allow 1 pixel to be off, otherwise its much too strict
        if [tile[4 * x + y] for x, y in possibleWall].count(True) >= len(possibleWall) - 1:
            return True
    return False

def classify(x, y):
    pixels = [pix[4 * x + i, 4 * y + j] for i in range(4) for j in range(4)]
    # snow would be counted as walls, lets avoid that
    hasSnow = True in [[distance(kindaGraySnow, pixel) < 50 for pixel in pixels].count(True) > 8]
    if hasSnow:
        # Let's assume you can walk on snow because there is too much to remove manually otherwise
        return 0
    # Check for 8 out of 16 pixels matching
    hasWaterOrLava = True in [[distance(baseColor, pixel) < 50 for pixel in pixels].count(True) > 8 for baseColor in (baseLavaColor, baseWaterColor)]
    if hasWaterOrLava:
        return EAST | WEST | NORTH | SOUTH
    # This is the criteria to turn a pixel into a boolean that dictates if we think there is a wall piece on that pixel
    # We check if the color value (0 to 255 RGB) is greater than 150 on R, G & B
    # Range 4 is because each tile is 4x4 pixels
    tile = [False not in [c > 150 for c in pix[4 * x + i, 4 * y + j]] for i in range(4) for j in range(4)]
    if hasWall(tile, diagonalsnegxy) or hasWall(tile, diagonalsxy):
        # return walls surrounding the tile if it has a diagonal wall on it
        return EAST | WEST | NORTH | SOUTH
    # For every remaining wall possibility we OR the bits since you
    # Can step on a tile from a certain direction even if it has a wall
    # You need the information on all walls surrounding the tiles
    walls = 0
    if hasWall(tile,  eastWalls): walls |=  EAST
    if hasWall(tile,  westWalls): walls |=  WEST
    if hasWall(tile, southWalls): walls |= SOUTH
    if hasWall(tile, northWalls): walls |= NORTH
    return walls

def waller(cx, cy):
    with open(f"../resource/chunks-{TPC}/walls/{cx}-{cy}-0.txt","w+") as f:
        walls = [classify(x + TPC * cx, y + TPC * cy) for x, y in TILES]
        wmode = mode(walls)[0][0]
        f.write(f"{wmode}\n")
        for x, y in TILES:
            w = walls[TPC * x + y]

            # The following process is to facilitate the pathfinding algorithm
            # Essentially so far, we might have a tile A next to B from west to east
            # A may have a wall on its east side and B no wall on its west side
            # We just check each surrounding tile and duplicate the walls on their neighbors
            # This way when pathfinding we will just have to look at our walls
            # and assume the neighbor's walls are consistent with ours

            # TODO
            # This process is done per chunk, so tile neighbors in
            # different chunks Are ignored, this is a HUGE flaw
            # One idea is to reconcialiate both files as you go, this would fit better
            # Than doing once each file is processed with the current state of chunker.py
            # The idea is that you could always try to reconciliate only with the chunks
            # west or north of you, eventually leading to the last chunk, all the way
            # south east, closing the last borders

            # TODO
            # only write down the tiles that have walls on them, not zeros

            if x >  0:
                neighbor = walls[TPC * (x - 1) + y]
                if neighbor &  EAST:
                    w |=  WEST
            if x < 63:
                neighbor = walls[TPC * (x + 1) + y]
                if neighbor &  WEST:
                    w |=  EAST
            if y >  0:
                neighbor = walls[TPC * x + (y - 1)]
                if neighbor & SOUTH:
                    w |= NORTH
            if y < 63:
                neighbor = walls[TPC * x + (y + 1)]
                if neighbor & NORTH:
                    w |= SOUTH
            if w != wmode:
                f.write(f"{x}-{y}:{w}\n")
