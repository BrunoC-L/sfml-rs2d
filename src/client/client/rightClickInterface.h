#pragma once
#include <SFML/Graphics.hpp>
#include "units.h"
#include "mouseEvent.h"
#include "abstractServices.h"
#include "serviceProvider.h"
#include "rightClickTileEvent.h"
#include "service.h"
#include "abstractRenderWindow.h"

class RightClickInterface : public Service {
private:
    AbstractRenderWindow* window;
    std::vector<ObjectInteractions> interactions;
    sf::Text text;
    sf::Font font;
    sf::RectangleShape rect;
    EventObserver<RightClickTileEvent> rightClickTileObserver;
    Tile* tile;
public:
    RightClickInterface(ServiceProvider* provider, AbstractRenderWindow* window);
    void draw();
    bool active;
    void setPosition(VPixel pos);
    bool mouseIsInRect(MouseEvent& ev);
    VPixel getDeltaClick(MouseEvent& ev);
    void click(MouseEvent& ev);
    void addText(std::string text);
    void resetText();
    void addInteractions(ObjectInteractions interactions);
};
