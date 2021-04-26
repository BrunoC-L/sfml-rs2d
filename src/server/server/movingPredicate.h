#pragma once
#include "units.h"
#include "chunk.h"
#include "abstractMap.h"


class MovingPredicate {
public:
    static bool canMoveFromTo(VTile a, VTile b, AbstractMap* map) {
        VChunk ca = VChunk(int(a.x / Chunk::TilesPerChunk), int(a.y / Chunk::TilesPerChunk));
        VChunk cb = VChunk(int(b.x / Chunk::TilesPerChunk), int(b.y / Chunk::TilesPerChunk));
        std::shared_ptr<Chunk> tileAChunk = map->getChunk(ca);
        std::shared_ptr<Chunk> tileBChunk = map->getChunk(cb);
        if (!tileAChunk || !tileBChunk)
            return false;
        std::shared_ptr<Tile> ta = tileAChunk->tiles[int(a.x - ca.x * Chunk::TilesPerChunk)][int(a.y - ca.y * Chunk::TilesPerChunk)];
        std::shared_ptr<Tile> tb = tileBChunk->tiles[int(b.x - cb.x * Chunk::TilesPerChunk)][int(b.y - cb.y * Chunk::TilesPerChunk)];
        return ta->canMoveFrom(*tb);
    }

    static std::vector<VTile> getNextTo(std::vector<VTile> tiles) {
        std::vector<VTile> adjacent = {};
        for (auto t : tiles) {
            adjacent.push_back(t + VTile( 0,  1));
            adjacent.push_back(t + VTile(-1,  0));
            adjacent.push_back(t + VTile( 0, -1));
            adjacent.push_back(t + VTile( 1,  0));
        }
        return adjacent;
    }

    static bool tileIsInVector(VTile t, std::vector<VTile> v) {
        return distance(t, v) == 0;
    };

    static float distance(VTile t, std::vector<VTile> v) {
        float best = 1000;
        for (auto target : v)
            if (!(best = std::min(best, distance(t, target))))
                break;
        return best;
    }

    static float distance(VTile t1, VTile t2) {
        if (t1 == t2)
            return 0;
        auto delta = t1 - t2;
        delta = VTile(delta.x > 0 ? delta.x : -delta.x, delta.y > 0 ? delta.y : -delta.y);
        if (delta.x == delta.y)
            return 0.5f + (delta.x > delta.y ? delta.x : delta.y);
        return delta.x > delta.y ? delta.x : delta.y;
    }
};
