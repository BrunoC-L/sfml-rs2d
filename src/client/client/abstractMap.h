#pragma once
#include "../../common/abstractService.h"
#include "../../common/units.h"
#include <vector>
#include "chunk.h"
using namespace std;

class AbstractMap : public AbstractService {
public:
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
