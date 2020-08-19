#pragma once
#include <SFML/Graphics.hpp>
#include "chunk.h"
#include "measures.h"
#include "Textures.h"
#include "getRenderWindow.h"
#include "player.h"
#include "camera.h"

using namespace std;
using namespace sf;

class Map {
    Map();
public:
    Map(const Map& other) = delete;
    Map operator=(const Map& other) = delete;
    static Map& getInstance() {
        static Map instance;
        return instance;
    }
    void load();
    void draw();
    void update();
    void updateChunks(const VChunk& difference, const VChunk& tempCenter);
    void doUpdates() {
        while (true) {
            if (shouldStop) return;
            update();
        }
    }
    Tile* getTileFromVTile(VTile tilePosition);
    bool shouldUpdate = false;
    bool shouldStop = false;
    unsigned chunkRadius = 0;
    VChunk centerChunk;
    vector<vector<Chunk*>> loaded;
    Mutex mutex;
};
