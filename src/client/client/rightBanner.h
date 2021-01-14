#pragma once
#include <SFML/Graphics.hpp>
#include "../../common/common/units.h"
#include "abstractMeasures.h"
#include "abstractPlayer.h"
#include "mouseEvent.h"
#include "minimap.h"
#include "item.h"
#include "service.h"

using namespace std;

class RightBanner : public Service {
public:
    RightBanner(AbstractServiceProvider* provider, AbstractRenderWindow* window);
    void draw();
    bool mouseIsInRect(MouseEvent* ev);
    void click(MouseEvent* ev);
    void update();
private:
    AbstractRenderWindow* window;
    Minimap minimap;
    RightBanner();
    sf::RectangleShape banner;
    vector<pair<Item, sf::RectangleShape>> inventoryItems;
};
