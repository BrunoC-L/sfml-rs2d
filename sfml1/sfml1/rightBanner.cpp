#include "rightBanner.h"

RightBanner::RightBanner(RenderWindow& w, Measures& measures) : w(w), measures(measures) {
    banner = RectangleShape(Vector2f(measures.rightBannerWidth, measures.getWindowSize().y));
    banner.setPosition(measures.getRightBannerStartingX(), 0);
}

void RightBanner::update() {
}

void RightBanner::draw() const {
    const auto scale = measures.stretch;
    w.draw(banner, RenderStates().transform.scale(Vector2f(1 / scale.x, 1)).translate(measures.getWindowSize().x - measures.startingScreenSize.x, 0));
}
