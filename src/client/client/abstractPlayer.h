#pragma once
#include <functional>
#include "../../common/abstractService.h"

class AbstractPlayer : public AbstractService {
public:
    int id = 0;
    virtual void update(unsigned tick) = 0;
    virtual void onGameTick() = 0;
    virtual void subscribeToTeleport() = 0;
    virtual void teleport(VTile pos) = 0;
    virtual void walk(VTile pos) = 0;
    VTile position;
};
