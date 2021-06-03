#pragma once
#include <SFML/Graphics.hpp>
#include "units.h"
#include "service.h"
#include "serviceProvider.h"
#include "abstractRenderWindow.h"

class BottomBanner : public Service {
public:
    BottomBanner(ServiceProvider* provider, AbstractRenderWindow* window);
    void draw() const;
    void click(MouseEvent& ev);
    bool mouseIsInRect(MouseEvent& ev);
private:
    AbstractRenderWindow* window;
    AbstractChat* chat;
    sf::RectangleShape banner;
};