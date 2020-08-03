#pragma once
#include <SFML/Graphics.hpp>
#include "units.h"
#include "measures.h"
using namespace std;
using namespace sf;

class Player {
public:
    Player(RenderWindow& w, Measures& measures, VTile position);
    void draw() const;
    void update();
    VTile position;
private:
    Texture t;
    Measures& measures;
    RenderWindow& w;
    Sprite player;
};