#include "minimap.h"

Minimap::Minimap(RenderWindow& w, VTile& pos, Measures& measures) : w(w), pos(pos), lastPos(-1, -1), measures(measures) {
    const int radius = Measures::minimapRadius;
    shape = CircleShape(radius);
    shape.setPosition(measures.getRightBannerStartingX() + (measures.banners.x - 2 * radius / measures.stretch.x) / 2, 0);
    minimap.loadFromFile("../../assets/mapnoraids.jpg");
    shape.setTexture(&minimap);
}

void Minimap::update() {
    if (pos == lastPos)
        return;
    lastPos = pos;
    float r = Measures::minimapRadius;
    shape.setTextureRect(IntRect(pos.x * Measures::pixelsPerTileOnMap - r, pos.y * Measures::pixelsPerTileOnMap - r, 2 * r, 2 * r));
}

void Minimap::draw() const {
    const auto scale = measures.stretch;
    w.draw(shape, RenderStates().transform.scale(Vector2f(1/scale.x, 1/scale.y)).translate(measures.getWindowSize().x - measures.startingScreenSize.x, 0));
}
