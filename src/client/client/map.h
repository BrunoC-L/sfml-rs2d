#pragma once
#include <SFML/Graphics.hpp>
#include "../../common/units.h"
#include "item.h"
#include "abstractServices.h"
#include "service.h"

using namespace std;

class Map: public AbstractMap, public Service {
public:
    Map(AbstractServiceProvider* provider);
    void init();
    virtual void load();
    virtual void draw();
    virtual void update();
    virtual void updateChunks(const VChunk& difference, const VChunk& tempCenter);
    virtual void doUpdates() {
        while (true) {
            if (shouldStop) return;
            update();
        }
    }
    virtual Tile* getTileFromVTile(VTile tilePosition);
};
