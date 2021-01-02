#pragma once
#include <SFML/Graphics.hpp>
#include "units.h"
#include "abstractServices.h"
#include "teleportEvent.h"
#include "event.h"
#include "abstractRenderWindow.h"
#include "interactionClickEvent.h"
#include "pathfinder.h"

using namespace std;

class Player : public AbstractPlayer {
public:
    Player(AbstractServiceProvider* provider);
    void init();
    void draw() const;
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
