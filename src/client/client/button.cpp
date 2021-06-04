#include "button.h"

Button::Button(
    AnchoredOffsetTransform&& aot,
    VPixel size,
    const sf::Texture& texture
) : aot(std::move(aot)), size(size) {
    shape.setSize(sf::Vector2f(size.x, size.y));
    shape.setTexture(&texture);
}

Button::Button(AnchoredOffsetTransform&& aot, VPixel size, sf::Color color) : aot(std::move(aot)), size(size) {
    shape.setSize(sf::Vector2f(size.x, size.y));
    shape.setFillColor(color);
}

void Button::onClick(std::function<void()> onClick) {
    this->click = std::make_shared<ClickableComponent>(onClick, aot, size);
}

void Button::onOffHover(std::function<void()> onHover, std::function<void()> offHover) {
    this->hover = std::make_shared<HoverableComponent>(onHover, offHover, aot, size);
}

void Button::draw(AbstractRenderWindow& window) {
    window.draw(&shape, aot.getTransform());
}
