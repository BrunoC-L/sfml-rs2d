#pragma once
#include "abstractServiceMacro.h"
#include "units.h"
#include <vector>
#include "chunk.h"
using namespace std;

SERVICES;

class AbstractMap : public Service {
	SERVICE_MEMBERS;
    virtual void load() = 0;
    virtual void draw() = 0;
    virtual void update() = 0;
    virtual void updateChunks(const VChunk& difference, const VChunk& tempCenter) = 0;
    virtual void doUpdates() = 0;
    virtual Tile* getTileFromVTile(VTile tilePosition) = 0;
    bool shouldUpdate = false;
    bool shouldStop = false;
    unsigned chunkRadius = 0;
    VChunk centerChunk;
    vector<vector<Chunk*>> loaded;
    sf::Mutex mutex;
};
