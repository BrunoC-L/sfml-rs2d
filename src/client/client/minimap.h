#pragma once
#include <SFML/Graphics.hpp>
#include "../../common/units.h"
#include "../../common/abstractServiceProvider.h"
#include "abstractServices.h"
#include "service.h"

using namespace std;

class Minimap : public Service {
public:
    Minimap(AbstractServiceProvider* provider);
    void draw() const;
    void update();
private:
    sf::CircleShape playerCircle;
    VTile lastPos;
    sf::CircleShape shape;
    sf::Texture minimap;
};