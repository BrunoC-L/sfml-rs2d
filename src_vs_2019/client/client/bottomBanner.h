#pragma once
#include <SFML/Graphics.hpp>
#include "units.h"
#include "service.h"
#include "serviceProvider.h"
#include "abstractRenderWindow.h"
#include "mouseLeftClickEvent.h"

class BottomBanner : public Service {
public:
    BottomBanner(ServiceProvider* provider, AbstractRenderWindow* window);
    void draw() const;
    void click(const MouseLeftClickEvent::Data& ev) {

    }
    template <typename MouseEvent>
    bool mouseIsInRect(const MouseEvent& ev) {
        int pxFromBottomBorder = window->getSize().y - ev.pos.y;
        return pxFromBottomBorder <= AbstractMeasures::bottomBannerHeight;
    }
private:
    AbstractRenderWindow* window;
    AbstractChat* chat;
    sf::RectangleShape banner;
};
