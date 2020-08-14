#pragma once
#include "units.h"
#include "measures.h"
#include "map.h"

class MovingPredicate {
public:
    static bool canMoveFromTo(VTile a, VTile b) {
        Measures& measures = Measures::getInstance();
        Map& map = Map::getInstance();
        VChunk ca = VChunk(int(a.x / Measures::TilesPerChunk), int(a.y / Measures::TilesPerChunk));
        VChunk cb = VChunk(int(b.x / Measures::TilesPerChunk), int(b.y / Measures::TilesPerChunk));
        VChunk da = ca - map.centerChunk + VChunk(map.loaded.size() / 2, map.loaded.size() / 2);
        VChunk db = cb - map.centerChunk + VChunk(map.loaded.size() / 2, map.loaded.size() / 2);
        if (da.x < 0 || db.x < 0 || da.x >= map.loaded.size() || db.x >= map.loaded.size() || da.y < 0 || db.y < 0 || da.y >= map.loaded.size() || db.y >= map.loaded.size())
            return false;
        Chunk* tileAChunk = map.loaded[da.x][da.y];
        Chunk* tileBChunk = map.loaded[db.x][db.y];
        Tile* ta = tileAChunk->tiles[int(a.x - ca.x * Measures::TilesPerChunk)][int(a.y - ca.y * Measures::TilesPerChunk)];
        Tile* tb = tileBChunk->tiles[int(b.x - cb.x * Measures::TilesPerChunk)][int(b.y - cb.y * Measures::TilesPerChunk)];
        return ta->canMoveFrom(*tb);
    }

    static vector<VTile> getNextTo(vector<VTile> tiles) {
        vector<VTile> adjacent = {};
        for (auto t : tiles) {
            adjacent.push_back(t + VTile( 0,  1));
            adjacent.push_back(t + VTile(-1,  0));
            adjacent.push_back(t + VTile( 0, -1));
            adjacent.push_back(t + VTile( 1,  0));
        }
        return adjacent;
    }

    static bool tileIsInVector(VTile t, vector<VTile> v) {
        for (auto x : v)
            if (x == t)
                return true;
        return false;
    };
};
