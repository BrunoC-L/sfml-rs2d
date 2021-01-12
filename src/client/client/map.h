#pragma once
#include "../../common/units.h"
#include "item.h"
#include "abstractServices.h"
#include "service.h"
#include <thread>

using namespace std;

class Map: public AbstractMap, public Service {
    std::thread updateThread;
public:
    Map(AbstractServiceProvider* provider);
    void init();
    virtual void load();
    virtual void update();
    virtual void updateChunks(const VChunk& difference, const VChunk& tempCenter);
    virtual void doUpdates();
    virtual Tile* getTileFromVTile(VTile tilePosition);
    virtual void stopUpdates();
};
