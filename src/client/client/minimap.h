#pragma once
#include <SFML/Graphics.hpp>
#include "units.h"
#include "serviceProvider.h"
#include "abstractServices.h"
#include "service.h"
#include "abstractRenderWindow.h"

class Minimap : public Service {
public:
    Minimap(ServiceProvider* provider, AbstractRenderWindow* window);
    void draw() const;
    void update();
private:
    AbstractRenderWindow* window;
    sf::CircleShape playerCircle;
    VTile lastPos;
    sf::CircleShape shape;
    sf::Texture minimap;
};