#pragma once
#include <SFML/Graphics.hpp>
#include "units.h"
#include "measures.h"
#include "getRenderWindow.h"
#include "player.h"
#include "mouseEvent.h"
#include "minimap.h"
#include "pathfinder.h"

using namespace std;
using namespace sf;

class RightBanner {
public:
    RightBanner(RightBanner& other) = delete;
    RightBanner operator=(const RightBanner& other) = delete;
    static RightBanner& getInstance() {
        static RightBanner instance;
        return instance;
    }
    void draw();
    bool mouseIsInRect(MouseEvent* ev);
    void click(MouseEvent* ev);
    void update();
private:
    Minimap minimap;
    RightBanner();
    RectangleShape banner;
    vector<pair<Item, RectangleShape>> inventoryItems;
};
