#pragma once
#include <SFML/Graphics.hpp>
#include "units.h"
#include "measures.h"
using namespace std;
using namespace sf;

class Minimap {
public:
    Minimap(RenderWindow& w, VTile& pos, Measures& measures);
    void draw() const;
    void update();
private:
    CircleShape playerCircle;
    Measures& measures;
    RenderWindow& w;
    VTile& pos;
    VTile lastPos;
    CircleShape shape;
    Texture minimap;
};