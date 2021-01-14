#pragma once
#include "../../common/common/units.h"
#include "teleportEvent.h"
#include "interactionClickEvent.h"
#include "service.h"

using namespace std;

class Player : public AbstractPlayer, public Service {
private:
    void subscribeToTeleport();
    void subscribeToInteractionClick();
    void subscribeToWalkClick();
    void teleport(VTile pos);
    void walk(VTile pos);
public:
    Player(AbstractServiceProvider* provider);
    void init();
};
