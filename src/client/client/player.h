#pragma once
#include <SFML/Graphics.hpp>
#include "../../common/units.h"
#include "abstractServices.h"
#include "teleportEvent.h"
#include "../../common/event.h"
#include "abstractRenderWindow.h"
#include "interactionClickEvent.h"
#include "service.h"

using namespace std;

class Player : public AbstractPlayer, public Service {
public:
    Player(AbstractServiceProvider* provider);
    void init();
    void update(unsigned tick);
    void onGameTick();
    void clearActionIfNotBusy();
    void setActionIfNotBusy(function<bool()> action);
    void subscribeToTeleport();
    void teleport(VTile pos);
    void subscribeToInteractionClick();
    void interact(function<bool()> f);
    void subscribeToWalkClick();
    void walk(VTile pos);
};
