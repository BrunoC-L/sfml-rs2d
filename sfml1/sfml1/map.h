#pragma once
#include <SFML/Graphics.hpp>
#include "chunk.h"
#include "measures.h"
using namespace std;
using namespace sf;

class Map {
public:
    Map(RenderWindow& w, VTile& pos, Measures& measures, const unsigned chunkRadius);
    void draw() const;
    void update();
    void updateChunks(const VChunk& difference, const VChunk& tempCenter);
    void doUpdates() {
        while (true) {
            if (shouldStop) return;
            update();
        }
    }
    bool shouldUpdate = false;
    bool shouldStop = false;
private:
    const unsigned chunkRadius;
    Measures& measures;
    RenderWindow& w;
    VTile& pos;
    VChunk centerChunk;
    vector<vector<Chunk*>> loaded;
};