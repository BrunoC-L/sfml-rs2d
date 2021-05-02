#pragma once
#include <SFML/Graphics.hpp>
#include "units.h"
#include "abstractMeasures.h"
#include "abstractPlayer.h"
#include "mouseEvent.h"
#include "minimap.h"
#include "item.h"
#include "service.h"

class RightBanner : public Service {
private:
    AbstractRenderWindow* window;
    Minimap minimap;
    RightBanner();
    sf::RectangleShape banner;
public:
    RightBanner(ServiceProvider* provider, AbstractRenderWindow* window);
    void draw();
    bool mouseIsInRect(MouseEvent& ev);
    void click(MouseEvent& ev);
    void update();
};
