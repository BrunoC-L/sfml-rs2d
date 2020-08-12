#pragma once
#include <SFML/Graphics.hpp>
#include "units.h"
#include "measures.h"
#include "getRenderWindow.h"
#include "player.h"
using namespace std;
using namespace sf;

class Minimap {
public:
    Minimap();
    void draw() const;
    void update();
private:
    CircleShape playerCircle;
    VTile lastPos;
    CircleShape shape;
    Texture minimap;
};