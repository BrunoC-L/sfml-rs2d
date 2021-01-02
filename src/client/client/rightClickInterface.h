#pragma once
#include <SFML/Graphics.hpp>
#include "units.h"
#include "mouseEvent.h"
#include "abstractServices.h"
#include "abstractServiceProvider.h"
#include "rightClickTileEvent.h"

using namespace std;

class RightClickInterface {
public:
    SERVICE_MEMBERS;
    RightClickInterface(AbstractServiceProvider* provider);
    void draw();
    bool active;
    void setPosition(VPixel pos);
    bool mouseIsInRect(MouseEvent* ev);
    VPixel getDeltaClick(MouseEvent* ev);
    void addText(string text);
    void resetText();
    void click(MouseEvent* ev);
    void addInteractions(string objectName, vector<pair<string, function<bool()>>> interactions);
private:
    vector<pair<string,function<bool()>>> interactions;
    sf::Text text;
    sf::Font font;
    RightClickInterface();
    sf::RectangleShape rect;
};
