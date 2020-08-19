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
    rect.setPosition(Vector2f(pos.x, pos.y));
    text.setPosition(Vector2f(pos.x, pos.y));
}

void RightClickInterface::setText(string txt) {
    text.setString(text.getString() + '\n' + txt);
}

void RightClickInterface::resetText() {
    text.setString("Choose Option");
}

bool RightClickInterface::mouseIsInRect(shared_ptr<MouseEvent> mouseEvent) {
    auto rectPos = rect.getPosition();
    VPixel delta(mouseEvent->position.x - rectPos.x, mouseEvent->position.y - rectPos.y);
    return delta.x < 100 && delta.y < 100 && delta.x >= 0 && delta.y >= 0;
}

void RightClickInterface::draw() {
    if (!active)
        return;
    Measures& measures = Measures::getInstance();
    RenderWindow& w = RenderWindowSingleton::getInstance();
    const auto scale = measures.stretch;
    Transform transform;
    transform.scale(Vector2f(1 / scale.x, 1 / scale.y));
    w.draw(rect, transform);
    w.draw(text, transform);
}

void RightClickInterface::click(shared_ptr<MouseEvent> mouseEvent) {

}
