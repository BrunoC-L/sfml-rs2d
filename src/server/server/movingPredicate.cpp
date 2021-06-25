#include "movingPredicate.h"
#include "map.h"

bool MovingPredicate::canMoveFromTo(VTile a, VTile b, Map* map) {
    VChunk ca = VChunk(int(a.x / TilesPerChunk), int(a.y / TilesPerChunk));
    VChunk cb = VChunk(int(b.x / TilesPerChunk), int(b.y / TilesPerChunk));
    std::shared_ptr<Chunk> tileAChunk = map->getChunk(ca);
    std::shared_ptr<Chunk> tileBChunk = map->getChunk(cb);
    if (!tileAChunk || !tileBChunk)
        return false;
    Tile* ta = tileAChunk->tiles[int(a.x - ca.x * TilesPerChunk)][int(a.y - ca.y * TilesPerChunk)];
    Tile* tb = tileBChunk->tiles[int(b.x - cb.x * TilesPerChunk)][int(b.y - cb.y * TilesPerChunk)];
    return ta->canMoveFrom(*tb);
}