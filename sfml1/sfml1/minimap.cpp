#include "minimap.h"

Minimap::Minimap() {
    Measures& measures = Measures::getInstance();
    shape = CircleShape(Measures::minimapRadius);
    shape.setPosition(measures.getRightBannerStartingX() + (measures.banners.x - 2 * Measures::minimapRadius / measures.stretch.x) / 2, 0);
    minimap.loadFromFile("../../assets/mapnoraids.jpg");
    shape.setTexture(&minimap);
    playerCircle = CircleShape(2);
    playerCircle.setPosition(shape.getPosition().x + Measures::minimapRadius - 2, Measures::minimapRadius - 2);
    playerCircle.setFillColor(Color::White);
}

void Minimap::update() {
    VTile& pos = Player::getInstance().position;
    if (pos == lastPos)
        return;
    lastPos = pos;
    float r = Measures::minimapRadius;
    shape.setTextureRect(IntRect(pos.x * Measures::pixelsPerTileOnMap - r, pos.y * Measures::pixelsPerTileOnMap - r, 2 * r, 2 * r));
}

void Minimap::draw() const {
    Measures& measures = Measures::getInstance();
    RenderWindow& w = RenderWindowSingleton::getInstance();
    const auto scale = measures.stretch;
    auto transform = Transform();
    transform
        .scale(Vector2f(1 / scale.x, 1 / scale.y))
        .translate(measures.getWindowSize().x - measures.startingScreenSize.x, 0)
        .rotate(measures.angle, Vector2f(shape.getPosition().x + Measures::minimapRadius, shape.getPosition().y + Measures::minimapRadius));
    w.draw(shape, transform);
    w.draw(playerCircle, transform);
}
