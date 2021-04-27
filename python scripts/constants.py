PPT = pixels_per_tile = 32
TPC = tiles_per_chunk = 64
FAC = 256 // PPT # The original map has 256 pixels per tile
assert FAC == 256 / PPT, f"Found PPT = {PPT}, Expected a factor of 256"

EAST  = 8
NORTH = 4
WEST  = 2
SOUTH = 1

CHUNKS = [(x, y) for x in range(29) for y in range(25)]
TILES = [(x, y) for x in range(TPC) for y in range(TPC)]
PIXELS = [(x, y) for x in range(PPT) for y in range(PPT)]
CHUNKS = [(18, 13)]
