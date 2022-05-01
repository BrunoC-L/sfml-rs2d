#pragma once
#include <SFML/Graphics.hpp>
#include "abstractService.h"
#include "units.h"
#include <vector>
#include <mutex>

class Chunk;
class Tile;
class ObjectInteractions;

class AbstractMap : public AbstractService {
public:
    virtual void load() = 0;
    virtual void update() = 0;
    virtual void updateChunks(const VChunk& difference, const VChunk& tempCenter) = 0;
    virtual void doUpdates() = 0;
    virtual void stopUpdates() = 0;
    virtual std::shared_ptr<Tile> getTileFromVTile(VTile tilePosition) = 0;
    virtual VChunk getCenterChunk()= 0;
    virtual Chunk& getLoaded(int i, int j) = 0;
    virtual std::mutex& getChunksMutex() = 0;
    virtual bool ready() = 0;
    virtual int* getObjectsPtrForChunk(VChunk chunk) = 0;
    virtual void updateInteractions(VChunk vc, VTile tile, ObjectInteractions* interactions) = 0;
    virtual Chunk& getChunk(VChunk chunk) = 0;
};
