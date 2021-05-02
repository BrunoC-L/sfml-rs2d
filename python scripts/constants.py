PPT = pixels_per_tile = 32
TPC = tiles_per_chunk = 64
ORIGINAL_MAP_PIXELS_PER_TILE = 256
OBJECT_TEXTURE_FILE_SIZE = 256
FAC = ORIGINAL_MAP_PIXELS_PER_TILE // PPT
assert FAC == ORIGINAL_MAP_PIXELS_PER_TILE / PPT, f"Found PPT = {PPT}, Expected a factor of {ORIGINAL_MAP_PIXELS_PER_TILE}"

EAST  = 8
NORTH = 4
WEST  = 2
SOUTH = 1

TILES = [(x, y) for x in range(TPC) for y in range(TPC)]
PIXELS = [(x, y) for x in range(PPT) for y in range(PPT)]

CHUNKS = [(x, y) for x in range(29) for y in range(25)]

LUMBRIDGE = (18, 13)
# CHUNKS = [LUMBRIDGE]