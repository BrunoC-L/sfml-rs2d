#pragma once
#include "units.h"
#include "chunk.h"
#include "abstractMap.h"


class MovingPredicate {
public:
    static bool canMoveFromTo(VTile a, VTile b, AbstractMap* map) {
        VChunk ca = VChunk(int(a.x / TILES_PER_CHUNK), int(a.y / TILES_PER_CHUNK));
        VChunk cb = VChunk(int(b.x / TILES_PER_CHUNK), int(b.y / TILES_PER_CHUNK));
        std::shared_ptr<Chunk> tileAChunk = map->getChunk(ca);
        std::shared_ptr<Chunk> tileBChunk = map->getChunk(cb);
        if (!tileAChunk || !tileBChunk)
            return false;
        Tile* ta = tileAChunk->tiles[int(a.x - ca.x * TILES_PER_CHUNK)][int(a.y - ca.y * TILES_PER_CHUNK)];
        Tile* tb = tileBChunk->tiles[int(b.x - cb.x * TILES_PER_CHUNK)][int(b.y - cb.y * TILES_PER_CHUNK)];
        return ta->canMoveFrom(*tb);
    }

    static std::vector<VTile> getNextTo(std::vector<VTile> tiles) {
        std::vector<VTile> adjacent = {};
        for (auto t : tiles)
            for (auto delta : { VTile(0, 1), VTile(-1, 0), VTile(0, -1), VTile(1, 0) })
                if (std::find(tiles.begin(), tiles.end(), t + delta) == tiles.end() && std::find(adjacent.begin(), adjacent.end(), t + delta) == adjacent.end())
                    adjacent.push_back(t + delta);
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
