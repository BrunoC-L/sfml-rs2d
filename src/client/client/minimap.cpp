#include "minimap.h"
#include "abstractMeasures.h"
#include "abstractPlayer.h"
#include "abstractRenderWindow.h"
#include "session.h"

Minimap::Minimap(ServiceProvider* provider, AbstractRenderWindow* window) : Service(provider), window(window) {
    acquire();
    shape = sf::CircleShape(AbstractMeasures::minimapRadius);
    shape.setPosition(measures->getRightBannerStartingX() + (AbstractMeasures::banners().x - 2 * AbstractMeasures::minimapRadius / measures->stretch.x) / 2, 0);
    minimap.loadFromFile(getSession().get("RS2D_HOME").asString() + "/assets/mapnoraids.jpg");
    shape.setTexture(&minimap);
    playerCircle = sf::CircleShape(2);
    playerCircle.setPosition(shape.getPosition().x + AbstractMeasures::minimapRadius - 2, AbstractMeasures::minimapRadius - 2);
    playerCircle.setFillColor(sf::Color::White);
}

void Minimap::update() {
    auto pos = player->getPosition();
    if (pos == lastPos)
        return;
    lastPos = pos;
    float r = AbstractMeasures::minimapRadius;
    shape.setTextureRect(sf::IntRect(pos.x * PIXELS_PER_TILE_ON_MAP - r, pos.y * PIXELS_PER_TILE_ON_MAP - r, 2 * r, 2 * r));
}

void Minimap::draw() const {
    const auto scale = measures->stretch;
    auto transform = sf::Transform();
    transform
        .scale(sf::Vector2f(1 / scale.x, 1 / scale.y))
        .translate(measures->windowSize.x - AbstractMeasures::startingScreenSize().x, 0)
        .rotate(measures->angle, sf::Vector2f(shape.getPosition().x + AbstractMeasures::minimapRadius, shape.getPosition().y + AbstractMeasures::minimapRadius));
    window->draw(&shape, transform);
    window->draw(&playerCircle, transform);
}
