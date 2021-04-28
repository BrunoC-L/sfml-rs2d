#include "rightClickInterface.h"
#include "abstractRenderWindow.h"

RightClickInterface::RightClickInterface(ServiceProvider* provider, AbstractRenderWindow* window) : Service(provider), window(window) {
    acquire();
    active = false;
    AbstractMeasures& measures = *(AbstractMeasures*)nullptr;
    rect = sf::RectangleShape(sf::Vector2f(100, 100));
    font.loadFromFile("../../../assets/runescape_uf.ttf");
    text.setFont(font);
    text.setFillColor(sf::Color::Red);
    text.setCharacterSize(16);
    resetText();
    rightClickTileObserver.set([&](RightClickTileEvent& ev) {
        if (active)
            return;
        active = true;
        resetText();
    });
}

void RightClickInterface::setPosition(VPixel pos) {
    interactions.clear();
    rect.setPosition(sf::Vector2f(pos.x, pos.y));
    text.setPosition(sf::Vector2f(pos.x, pos.y));
}

void RightClickInterface::addText(std::string txt) {
    text.setString(text.getString() + '\n' + txt);
}

void RightClickInterface::resetText() {
    text.setString("Choose Option");
}

bool RightClickInterface::mouseIsInRect(MouseEvent& ev) {
    auto delta = getDeltaClick(ev);
    return delta.x < rect.getSize().x && delta.y < rect.getSize().y && delta.x >= 0 && delta.y >= 0;
}

VPixel RightClickInterface::getDeltaClick(MouseEvent& ev) {
    auto rectPos = rect.getPosition();
    return VPixel(ev.pos.x - rectPos.x, ev.pos.y - rectPos.y);
}

void RightClickInterface::draw() {
    if (!active)
        return;
    rect.setSize(sf::Vector2f(100, (interactions.size() + 1) * 16 + 6));
    const auto scale = measures->stretch;
    sf::Transform transform;
    transform.scale(sf::Vector2f(1 / scale.x, 1 / scale.y));
    window->draw(&rect, transform);
    window->draw(text, transform);
}

void RightClickInterface::click(MouseEvent& ev) {
}

void RightClickInterface::addInteractions(std::string objectName, std::vector<std::pair<std::string, std::function<bool()>>> interactions) {
    for (auto i : interactions) {
        addText(objectName + '\t' + i.first);
        this->interactions.push_back(i);
    }
}
