#include "rightClickInterface.h"
#include "abstractRenderWindow.h"
#include "tile.h"
#include "session.h"

RightClickInterface::RightClickInterface(ServiceProvider* provider, AbstractRenderWindow* window) : Service(provider), window(window) {
    acquire();
    active = false;
    AbstractMeasures& measures = *(AbstractMeasures*)nullptr;
    rect = sf::RectangleShape(sf::Vector2f(100, 100));
    font.loadFromFile(getSession().get("RS2D_HOME").asString() + "/assets/runescape_uf.ttf");
    text.setFont(font);
    text.setFillColor(sf::Color::Red);
    text.setCharacterSize(16);
    resetText();
    rightClickTileObserver.set([&](const RightClickTileEvent::Data& ev) {
        if (active)
            return;
        active = true;
        resetText();
        for (const auto& i : ev.tile->getInteractions())
            addInteractions(i);
        tile = ev.tile;
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

void RightClickInterface::addInteractions(ObjectInteractions interactions) {
    for (auto i : interactions.interactions) {
        addText(interactions.objectName + '\t' + i);
        this->interactions.push_back(ObjectInteractions(interactions.tile, interactions.objectName, { i }, interactions.objectState));
    }
}
