from PIL import Image
from constants import PPT, FAC, TPC, TILES, PIXELS

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
            for i, j in PIXELS:
                px, py = (PPT * x + i, PPT * y + j)
                pxo, pyo = ((PPT * (TPC * cx + x) + i) // FAC, (PPT * (TPC * cy + y) + j) // FAC)
                pixout[px, py] = pixin[pxo, pyo]
        output.save(f"../assets/textures/chunks/{cx}-{cy}-0.png")
