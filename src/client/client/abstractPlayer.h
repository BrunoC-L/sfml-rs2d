#pragma once
#include "abstractServiceMacro.h"
#include <functional>
#include <SFML/Graphics.hpp>

SERVICES

class AbstractPlayer : public Service {
    SERVICE_MEMBERS;
    virtual void draw() const = 0;
    virtual void update(unsigned tick) = 0;
    virtual void onGameTick() = 0;
    virtual void clearActionIfNotBusy() = 0;
    virtual void setActionIfNotBusy(function<bool()> action) = 0;
    virtual void subscribeToTeleport() = 0;
    virtual void teleport(VTile pos) = 0;
    VTile position;
    VTile positionLastTick;
    VTile positionNextTick;
    VTile currentMovement[2];
    vector<VTile> path = {};
    bool isBusy;
    function<bool()> currentAction;
    sf::Texture t;
    sf::Sprite playerSprite;
};
