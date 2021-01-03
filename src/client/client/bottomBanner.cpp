#include "bottomBanner.h"

BottomBanner::BottomBanner(AbstractServiceProvider* provider) : Service(provider) {
    acquire();
    banner = sf::RectangleShape(sf::Vector2f(measures->getWindowSize().x, measures->bottomBannerHeight));
    banner.setPosition(0, measures->getBottomBannerStartingY());
}

void BottomBanner::draw() const {
    const auto scale = measures->stretch;
    renderWindow->draw(
        &banner,
        sf::RenderStates().transform.scale(sf::Vector2f(1, 1 / scale.y)).translate(0, measures->getWindowSize().y - AbstractMeasures::startingScreenSize().y)
    );
}
