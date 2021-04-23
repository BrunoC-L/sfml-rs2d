#pragma once
#include <SFML/Graphics.hpp>
#include "units.h"
#include "abstractServiceProvider.h"
#include "abstractServices.h"
#include "service.h"
#include "abstractRenderWindow.h"

using namespace std;

class Minimap : public Service {
public:
    Minimap(AbstractServiceProvider* provider, AbstractRenderWindow* window);
    void draw() const;
    void update();
private:
    AbstractRenderWindow* window;
    sf::CircleShape playerCircle;
    VTile lastPos;
    sf::CircleShape shape;
    sf::Texture minimap;
};