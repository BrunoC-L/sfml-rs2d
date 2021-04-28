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
    virtual unsigned getRadius() = 0;
    virtual VChunk getCenterChunk()= 0;
    virtual Chunk& getLoaded(int i, int j) = 0;
    virtual std::mutex& getChunksMutex() = 0;
};
