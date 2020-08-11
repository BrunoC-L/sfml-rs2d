#include "player.h"

Player::Player(RenderWindow& w, Measures& measures, VTile position) : w(w), measures(measures), position(position) {
    t.loadFromFile("../../assets/player.png");
    player = Sprite(t);
    currentMovement[0] = VTile();
    currentMovement[1] = VTile();
    positionLastTick = position;
    positionNextTick = position;
    currentAction = make_pair(false, []() { return true; });
}

void Player::update(unsigned tickmod) {
    if (currentMovement[0] == VTile())
        return;
    if (currentMovement[1] == VTile()) {
        position = positionLastTick + currentMovement[0] * ((tickmod + 1) / 36.f);
        return;
    }
    if (tickmod < 18) {
        position = positionLastTick + currentMovement[0] * ((tickmod + 1) / 18.f);
        return;
    }
    position = positionLastTick + currentMovement[0] + currentMovement[1] * ((tickmod % 18 + 1) / 18.f);
}

void Player::onGameTick(vector<VTile>& path) {
    if (currentAction.first && currentAction.second())
        currentAction = make_pair(false, []() { return true; });
    positionLastTick = position;
    if (path.size()) {
        currentMovement[0] = path[0] - position;
        path.erase(path.begin());
        if (path.size()) {
            currentMovement[1] = path[0] - (position + currentMovement[0]);
            path.erase(path.begin());
        }
        else {
            currentMovement[1] = VTile();
        }
    }
    else {
        currentMovement[0] = VTile();
        currentMovement[1] = VTile();
    }
    positionNextTick = positionLastTick + currentMovement[0] + currentMovement[1];
}

void Player::draw(VTile cameraPos) const {
    const auto zoomScale = measures.zoom;
    const auto scale = measures.stretch;
    w.draw(
        player,
        RenderStates().transform
            .scale(1 / scale.x, 1 / scale.y)
            .translate(measures.getInnerWindowSizeTile().x * Measures::pixelsPerTile / 2, measures.getInnerWindowSizeTile().y * Measures::pixelsPerTile / 2)
            .scale(zoomScale, zoomScale)
            .translate(-(17.f/16) * measures.getTileSize().x, - 1.5 * measures.getTileSize().y)
            .translate(position.x - cameraPos.x, position.y - cameraPos.y)
    );
}
