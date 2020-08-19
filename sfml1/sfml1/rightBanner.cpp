#include "rightBanner.h"

RightBanner::RightBanner() {
    Measures& measures = Measures::getInstance();
    banner = RectangleShape(Vector2f(measures.rightBannerWidth, measures.getWindowSize().y));
    banner.setPosition(measures.getRightBannerStartingX(), 0);
}

void RightBanner::draw() const {
    Measures& measures = Measures::getInstance();
    RenderWindow& w = RenderWindowSingleton::getInstance();
    const auto scale = measures.stretch;
    w.draw(banner, RenderStates().transform.scale(Vector2f(1 / scale.x, 1)).translate(measures.getWindowSize().x - measures.startingScreenSize.x, 0));
}
