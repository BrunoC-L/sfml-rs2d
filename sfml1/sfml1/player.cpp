#include "player.h"

Player::Player(RenderWindow& w, Measures& measures, VTile position) : w(w), measures(measures), position(position) {
    t.loadFromFile("../../assets/player.png");
    player = Sprite(t);
    update();
}

void Player::update() {
}

void Player::draw() const {
    const auto zoomScale = measures.zoom;
    const auto scale = measures.stretch;
    w.draw(
        player,
        RenderStates().transform
            .scale(1 / scale.x, 1 / scale.y)
            .translate(measures.getInnerWindowSizeTile().x * 16, measures.getInnerWindowSizeTile().y * 16)
            .scale(zoomScale, zoomScale)
            .translate(-1 * measures.getTileSize().x, - 1.5 * measures.getTileSize().y)
    );
}
