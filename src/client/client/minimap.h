#pragma once
#include <SFML/Graphics.hpp>
#include "units.h"
#include "abstractServiceProvider.h"
#include "abstractServices.h"

using namespace std;

class Minimap {
public:
    SERVICE_MEMBERS;
    Minimap(AbstractServiceProvider* provider);
    void draw() const;
    void update();
private:
    sf::CircleShape playerCircle;
    VTile lastPos;
    sf::CircleShape shape;
    sf::Texture minimap;
};