#include "rightClickInterface.h"

RightClickInterface::RightClickInterface() {
    active = false;
    Measures& measures = Measures::getInstance();
    rect = RectangleShape(Vector2f(100, 100));
    font.loadFromFile("../../assets/runescape_uf.ttf");
    text.setFont(font);
    text.setFillColor(sf::Color::Red);
    text.setCharacterSize(16);
    resetText();
}

void RightClickInterface::setPosition(VPixel pos) {
    interactions.clear();
    rect.setPosition(Vector2f(pos.x, pos.y));
    text.setPosition(Vector2f(pos.x, pos.y));
}

void RightClickInterface::addText(string txt) {
    text.setString(text.getString() + '\n' + txt);
}

void RightClickInterface::resetText() {
    text.setString("Choose Option");
}

bool RightClickInterface::mouseIsInRect(MouseEvent* ev) {
    auto delta = getDeltaClick(ev);
    return delta.x < rect.getSize().x && delta.y < rect.getSize().y && delta.x >= 0 && delta.y >= 0;
}

VPixel RightClickInterface::getDeltaClick(MouseEvent* ev) {
    auto rectPos = rect.getPosition();
    return VPixel(ev->pos.x - rectPos.x, ev->pos.y - rectPos.y);
}

void RightClickInterface::draw() {
    if (!active)
        return;
    rect.setSize(Vector2f(100, (interactions.size() + 1) * 16 + 6));
    Measures& measures = Measures::getInstance();
    RenderWindow& w = RenderWindowSingleton::getInstance();
    const auto scale = measures.stretch;
    Transform transform;
    transform.scale(Vector2f(1 / scale.x, 1 / scale.y));
    w.draw(rect, transform);
    w.draw(text, transform);
}

void RightClickInterface::click(MouseEvent* ev) {
    active = false;
    if (!interactions.size())
        return;
    Player& player = Player::getInstance();
    auto dy = getDeltaClick(ev).y - 16;
    int interactionIndex = dy / 16;
    if (dy < 0 || interactionIndex >= interactions.size())
        return;
    pair<string, function<bool()>> interaction = interactions[interactionIndex];
    if (interaction.first == "Examine")
        interaction.second();
    else
        player.setActionIfNotBusy(interaction.second);
}

void RightClickInterface::addInteractions(string objectName, vector<pair<string, function<bool()>>> interactions) {
    for (auto i : interactions) {
        addText(objectName + '\t' + i.first);
        this->interactions.push_back(i);
    }
}
