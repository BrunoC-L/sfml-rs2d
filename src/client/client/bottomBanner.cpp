#include "bottomBanner.h"

BottomBanner::BottomBanner(ServiceProvider* provider, AbstractRenderWindow* window) : Service(provider), window(window) {
    acquire();
    banner = sf::RectangleShape(sf::Vector2f(window->getSize().x, measures->bottomBannerHeight));
    banner.setPosition(0, measures->getBottomBannerStartingY());
    this->chat = (AbstractChat*)provider->get(SERVICES::CHAT);
}

void BottomBanner::draw() const {
    const auto scale = measures->stretch;
    window->draw(
        &banner,
        sf::RenderStates().transform.scale(sf::Vector2f(1, 1 / scale.y)).translate(0, window->getSize().y - AbstractMeasures::startingScreenSize().y)
    );
}

void BottomBanner::click(MouseEvent& ev) {
    socket->disconnect();
}

bool BottomBanner::mouseIsInRect(MouseEvent& ev) {
    int pxFromBottomBorder = window->getSize().y - ev.pos.y;
    return pxFromBottomBorder <= AbstractMeasures::bottomBannerHeight;
}
