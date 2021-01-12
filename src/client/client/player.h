#pragma once
#include "../../common/units.h"
#include "teleportEvent.h"
#include "interactionClickEvent.h"
#include "service.h"

using namespace std;

class Player : public AbstractPlayer, public Service {
public:
    Player(AbstractServiceProvider* provider);
    void init();
    void update(unsigned tick);
    void onGameTick();
    void subscribeToTeleport();
    void teleport(VTile pos);
    void subscribeToInteractionClick();
    void subscribeToWalkClick();
    void walk(VTile pos);
};
