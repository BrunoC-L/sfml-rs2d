import os, json

CONFIG = json.loads(open(os.environ['RS2D_HOME'] + '/config.json').read())

PPT = pixels_per_tile = CONFIG["PixelsPerTile"]
TPC = tiles_per_chunk = CONFIG["TilesPerChunk"]
ORIGINAL_MAP_PIXELS_PER_TILE = CONFIG["OriginalMapPixelsPerTile"]
OBJECT_TEXTURE_FILE_SIZE     = CONFIG["ObjectFileWidthHeightItems"]
MAP_SIZE = CONFIG["MapSize"]
MAP_SIZE_CHUNKS = {
    "x": MAP_SIZE["x"] // TPC,
    "y": MAP_SIZE["y"] // TPC,
}

EAST  = 8
NORTH = 4
WEST  = 2
SOUTH = 1

TILES  = [(x, y) for x in range(TPC) for y in range(TPC)]
PIXELS = [(x, y) for x in range(PPT) for y in range(PPT)]
CHUNKS = [(x, y) for x in range(MAP_SIZE_CHUNKS["x"]) for y in range(MAP_SIZE_CHUNKS["y"])]
# CHUNKS = [(x, y) for x in range(17, 20) for y in range(12, 15)]

with open('../src/common/common/constants.h', 'w+') as f:
    f.write('/*\nThis file was automatically generated, please do not modify manually, see "python scripts/constants.py"\n*/\n')
    f.write(f'#define MAX_PLAYERS_ONLINE {CONFIG["MaxPlayersOnline"]}\n')
    f.write(f'#define MSPT {CONFIG["MilliSecondsPerTick"]}\n')
    f.write(f'#define CHUNK_RADIUS {CONFIG["ChunkRadius"]}\n')
    f.write(f'#define PIXELS_PER_TILE {PPT}\n')
    f.write(f'#define PIXELS_PER_TILE_ON_MAP {ORIGINAL_MAP_PIXELS_PER_TILE}\n')
    f.write(f'#define TILES_PER_CHUNK {TPC}\n')
    f.write(f'#define TEXTURES_FOLDER std::string("textures-{PPT}")\n')
    f.write(f'#define CHUNKS_FOLDER std::string("chunks-{TPC}")\n')
