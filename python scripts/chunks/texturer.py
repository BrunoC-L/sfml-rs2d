from PIL import Image
from constants import PPT, ORIGINAL_MAP_PIXELS_PER_TILE, TPC, TILES, PIXELS

'''
This script will open the world map and generate a file for each chunk
Those resulting files will be loaded at runtime through the graphics engine
and displayed on the ground, it is literally the map
'''

def texturer(cx, cy):
    with Image.open('../assets/mapnoraids.jpg') as original:
        pixin = original.load()
        output = Image.new('RGB', (TPC * PPT, TPC * PPT))
        pixout = output.load()
        for x, y in TILES:
            inputImageTilePx = ORIGINAL_MAP_PIXELS_PER_TILE * (TPC * cx + x)
            inputImageTilePy = ORIGINAL_MAP_PIXELS_PER_TILE * (TPC * cy + y)
            for i, j in PIXELS:
                pxValue = pixin[
                    inputImageTilePx + (i * ORIGINAL_MAP_PIXELS_PER_TILE // PPT),
                    inputImageTilePy + (j * ORIGINAL_MAP_PIXELS_PER_TILE // PPT),
                ]

                px, py = (PPT * x + i, PPT * y + j)
                pixout[px, py] = pxValue
        output.save(f"../assets/textures-{PPT}/chunks-{TPC}/{cx}-{cy}-0.png")
