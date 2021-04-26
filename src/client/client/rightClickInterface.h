#pragma once
#include <SFML/Graphics.hpp>
#include "units.h"
#include "mouseEvent.h"
#include "abstractServices.h"
#include "serviceProvider.h"
#include "rightClickTileEvent.h"
#include "service.h"

class RightClickInterface : public Service {
public:
    RightClickInterface(ServiceProvider* provider, AbstractRenderWindow* window);
    void draw();
    bool active;
    void setPosition(VPixel pos);
    bool mouseIsInRect(MouseEvent* ev);
    VPixel getDeltaClick(MouseEvent* ev);
    void addText(std::string text);
    void resetText();
    void click(MouseEvent* ev);
    void addInteractions(std::string objectName, std::vector<std::pair<std::string, std::function<bool()>>> interactions);
private:
    AbstractRenderWindow* window;
    std::vector<std::pair<std::string, std::function<bool()>>> interactions;
    sf::Text text;
    sf::Font font;
    RightClickInterface();
    sf::RectangleShape rect;
};
