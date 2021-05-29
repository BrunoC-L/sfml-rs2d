#include "rightClickInterface.h"
#include "abstractRenderWindow.h"
#include "tile.h"

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
    active = false;
    if (!interactions.size())
        return;
    auto dy = getDeltaClick(ev).y - 16;
    int interactionIndex = dy / 16;
    if (dy < 0 || interactionIndex >= interactions.size())
        return;
    auto interaction = interactions[interactionIndex];
    JSON data;
    data["x"] = std::to_string(interaction.tile.x);
    data["y"] = std::to_string(interaction.tile.y);
    data["object"] = "{}";
    data["object"]["state"] = std::to_string(interaction.objectState);
    data["object"]["interaction"] = interaction.interactions[interactionIndex];
    data["object"]["objectName"] = interaction.objectName;
    socket->send("interact", data);
}

void RightClickInterface::addInteractions(ObjectInteractions interactions) {
    for (auto i : interactions.interactions) {
        addText(interactions.objectName + '\t' + i);
        this->interactions.push_back(ObjectInteractions(interactions.tile, interactions.objectName, { i }, interactions.objectState));
    }
}
