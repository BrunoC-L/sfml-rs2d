#pragma once
#include "abstractService.h"
#include "units.h"
#include <vector>
#include "chunk.h"
#include <mutex>

class AbstractMap : public AbstractService {
public:
    virtual void load() = 0;
    virtual void update() = 0;
    virtual void updateChunks(const VChunk& difference, const VChunk& tempCenter) = 0;
    virtual void doUpdates() = 0;
    virtual void stopUpdates() = 0;
    virtual std::shared_ptr<Tile> getTileFromVTile(VTile tilePosition) = 0;
    unsigned chunkRadius = 0;
    VChunk centerChunk;
    std::vector<std::vector<Chunk*>> loaded;
    std::mutex mutex;
};
