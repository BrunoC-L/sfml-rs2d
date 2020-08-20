#include "player.h"

Player::Player() : inventory(Inventory(28, {})) {
    t.loadFromFile("../../assets/player.png");
    player = Sprite(t);
    currentMovement[0] = VTile();
    currentMovement[1] = VTile();
    position = VTile();
    positionLastTick = position;
    positionNextTick = position;
    currentAction = []() { return false; };
}

void Player::update(unsigned tickmod) {
    if (currentMovement[0] == VTile())
        return;
    if (currentMovement[1] == VTile()) {
        position = positionLastTick + currentMovement[0] * ((tickmod + 1) / Measures::framesPerTick);
        return;
    }
    if (tickmod * 2 < Measures::framesPerTick ) {
        position = positionLastTick + currentMovement[0] * 2 * ((tickmod + 1) / Measures::framesPerTick);
        return;
    }
    position = positionLastTick + currentMovement[0] + currentMovement[1] * ((2 * tickmod) % unsigned(Measures::framesPerTick) + 2) / Measures::framesPerTick;
}

void Player::onGameTick() {
    if (!currentAction())
        currentAction =[]() { return false; };
    positionLastTick = position;
    if (path.size()) {
        currentMovement[0] = path[0] - position;
        path.erase(path.begin());
        if (path.size()) {
            currentMovement[1] = path[0] - (position + currentMovement[0]);
            path.erase(path.begin());
        }
        else
            currentMovement[1] = VTile();
    }
    else {
        currentMovement[0] = VTile();
        currentMovement[1] = VTile();
    }
    positionNextTick = positionLastTick + currentMovement[0] + currentMovement[1];
}

void Player::draw(VTile cameraPos) const {
    Measures& measures = Measures::getInstance();
    const auto zoomScale = measures.zoom;
    const auto scale = measures.stretch;
    RenderWindowSingleton::getInstance().draw(
        player,
        RenderStates().transform
            .scale(1 / scale.x, 1 / scale.y)
            .translate(measures.getInnerWindowSizeTile().x * Measures::pixelsPerTile / 2, measures.getInnerWindowSizeTile().y * Measures::pixelsPerTile / 2)
            .scale(zoomScale, zoomScale)
            .translate(-(17.f/16) * measures.getTileSize().x, - 1.5 * measures.getTileSize().y)
            .translate(position.x - cameraPos.x, position.y - cameraPos.y)
    );
}

void Player::clearActionIfNotBusy() {
    if (!isBusy)
        currentAction = []() { return true; };
}

void Player::setActionIfNotBusy(function<bool()> action) {
    if (!isBusy)
        currentAction = action;
}