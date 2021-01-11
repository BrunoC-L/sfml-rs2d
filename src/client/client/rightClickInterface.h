#pragma once
#include <SFML/Graphics.hpp>
#include "../../common/units.h"
#include "mouseEvent.h"
#include "abstractServices.h"
#include "../../common/abstractServiceProvider.h"
#include "rightClickTileEvent.h"
#include "service.h"

using namespace std;

class RightClickInterface : public Service {
public:
    RightClickInterface(AbstractServiceProvider* provider, AbstractRenderWindow* window);
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
    AbstractRenderWindow* window;
    vector<pair<string,function<bool()>>> interactions;
    sf::Text text;
    sf::Font font;
    RightClickInterface();
    sf::RectangleShape rect;
};
