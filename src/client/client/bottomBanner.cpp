#include "bottomBanner.h"

BottomBanner::BottomBanner(AbstractServiceProvider* provider, AbstractRenderWindow* window) : Service(provider), window(window) {
    acquire();
    banner = sf::RectangleShape(sf::Vector2f(window->getSize().x, measures->bottomBannerHeight));
    banner.setPosition(0, measures->getBottomBannerStartingY());
}

void BottomBanner::draw() const {
    const auto scale = measures->stretch;
    window->draw(
        &banner,
        sf::RenderStates().transform.scale(sf::Vector2f(1, 1 / scale.y)).translate(0, window->getSize().y - AbstractMeasures::startingScreenSize().y)
    );
}
