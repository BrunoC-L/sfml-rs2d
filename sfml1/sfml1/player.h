#pragma once
#include <SFML/Graphics.hpp>
#include "units.h"
#include "measures.h"
#include "getRenderWindow.h"
#include "inventory.h"

using namespace std;
using namespace sf;

class Player {
public:
    Player(const Player& other) = delete;
    Player operator=(const Player& other) = delete;
    static Player& getInstance() {
        static Player instance;
        return instance;
    }
    void draw(VTile cameraPosition) const;
    void update(unsigned tick);
    void onGameTick();
    VTile position;
    VTile positionLastTick;
    VTile positionNextTick;
    VTile currentMovement[2];
    vector<VTile> path = {};
    Inventory inventory;
    bool isBusy;
    void clearActionIfNotBusy();
    void setActionIfNotBusy(function<bool()> action);
private:
    function<bool()> currentAction;
    Player();
    Texture t;
    Sprite player;
};