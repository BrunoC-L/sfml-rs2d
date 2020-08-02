#include "bottomBanner.h"

BottomBanner::BottomBanner(RenderWindow& w, Measures& measures) : w(w), measures(measures) {
    banner = RectangleShape(Vector2f(measures.getWindowSize().x, measures.bottomBannerHeight));
    banner.setPosition(0, measures.getBottomBannerStartingY());
}

void BottomBanner::update() {
}

void BottomBanner::draw() const {
    const auto scale = measures.stretch;
    w.draw(banner, RenderStates().transform.scale(Vector2f(1, 1 / scale.y)).translate(0, measures.getWindowSize().y - measures.startingScreenSize.y));
}
