#include "button.h"
#include "font.h"

Button::Button(
    std::shared_ptr<AnchorTransform>&& aot,
    VPixel size,
    const sf::Texture& texture
) : aot(std::move(aot)), size(size) {
    shape.setSize(sf::Vector2f(size.x, size.y));
    shape.setTexture(&texture);
}

Button::Button(std::shared_ptr<AnchorTransform>&& aot, VPixel size, sf::Color color) : aot(std::move(aot)), size(size) {
    shape.setSize(sf::Vector2f(size.x, size.y));
    shape.setFillColor(color);
}

Button::Button(std::shared_ptr<AnchorTransform>&& aot, VPixel size) : aot(std::move(aot)), size(size) {
    shape.setSize(sf::Vector2f(size.x, size.y));
}

void Button::onClick(std::function<void()> onClick) {
    this->click = std::make_shared<ClickableComponent>(onClick, *aot, size);
}

void Button::onOffHover(std::function<void()> onHover, std::function<void()> offHover) {
    this->hover = std::make_shared<HoverableComponent>(onHover, offHover, *aot, size);
}

void Button::text(std::string&& text, sf::Color color) {
    strtext = std::move(text);
    sftext = sf::Text(strtext, font);
    sftext.setFillColor(color);
}

void Button::texture(const sf::Texture& texture) {
    shape.setTexture(&texture);
}

void Button::draw(AbstractRenderWindow& window) {
    auto tr = aot->getTransform();
    window.draw(&shape, tr);
    if (strtext.length())
        window.draw(sftext, tr);
}
