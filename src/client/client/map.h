#pragma once
#include "units.h"
#include "item.h"
#include "abstractServices.h"
#include "service.h"
#include <thread>

class Map: public AbstractMap, public Service {
    std::thread updateThread;
    bool shouldStop = false;
public:
    Map(ServiceProvider* provider);
    void init();
    virtual void load();
    virtual void update();
    virtual void updateChunks(const VChunk& difference, const VChunk& tempCenter);
    virtual void doUpdates();
    virtual std::shared_ptr<Tile> getTileFromVTile(VTile tilePosition);
    virtual void stopUpdates();
};
