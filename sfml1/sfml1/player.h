#pragma once
#include <SFML/Graphics.hpp>
#include "units.h"
#include "measures.h"

using namespace std;
using namespace sf;

class Player {
public:
    Player(RenderWindow& w, Measures& measures, VTile position);
    void draw(VTile cameraPosition) const;
    void update(unsigned tick);
    void onGameTick(vector<VTile>& path);
    VTile position;
    VTile positionLastTick;
    VTile positionNextTick;
    VTile currentMovement[2];
private:
    Texture t;
    Measures& measures;
    RenderWindow& w;
    Sprite player;
};