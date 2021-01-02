#pragma once
#include <SFML/Graphics.hpp>
#include "units.h"
#include "abstractMeasures.h"
#include "abstractPlayer.h"
#include "mouseEvent.h"
#include "minimap.h"
#include "pathfinder.h"
#include "item.h"

using namespace std;

class RightBanner {
public:
    SERVICE_MEMBERS;
    RightBanner(AbstractServiceProvider* provider);
    void draw();
    bool mouseIsInRect(MouseEvent* ev);
    void click(MouseEvent* ev);
    void update();
private:
    Minimap minimap;
    RightBanner();
    sf::RectangleShape banner;
    vector<pair<Item, sf::RectangleShape>> inventoryItems;
};
