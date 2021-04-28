#pragma once
#include "units.h"
#include "item.h"
#include "abstractServices.h"
#include "service.h"
#include <thread>

class Map: public AbstractMap, public Service {
    std::thread updateThread;
    bool shouldStop = false;
    unsigned chunkRadius;
    VChunk centerChunk;
    std::vector<std::vector<Chunk*>> loaded;
    std::mutex mutex;
public:
    Map(ServiceProvider* provider, int chunkRadius);
    void init();
    virtual void load();
    virtual void update();
    virtual void updateChunks(const VChunk& difference, const VChunk& tempCenter);
    virtual void doUpdates();
    virtual std::shared_ptr<Tile> getTileFromVTile(VTile tilePosition);
    virtual void stopUpdates();
    virtual unsigned getRadius() override;
    virtual VChunk getCenterChunk() override;
    virtual Chunk& getLoaded(int i, int j) override;
    virtual std::mutex& getChunksMutex() override;
};
