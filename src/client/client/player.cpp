#include "player.h"

Player::Player(AbstractServiceProvider* provider): Service(provider) {
    provider->set("Player", this);
    t.loadFromFile("../../../assets/player.png");
    playerSprite = sf::Sprite(t);
    currentMovement[0] = VTile();
    currentMovement[1] = VTile();
    position = VTile();
    positionLastTick = position;
    positionNextTick = position;
    currentAction = []() { return false; };
}

void Player::init() {
    acquire();
    VTile lumbridge(18 * AbstractMeasures::TilesPerChunk + 20, 13 * AbstractMeasures::TilesPerChunk + 37, 0);
    position = lumbridge;
    subscribeToTeleport();
    subscribeToInteractionClick();
    subscribeToWalkClick();
}

void Player::update(unsigned tickmod) {
    if (currentMovement[0] == VTile())
        return;
    if (currentMovement[1] == VTile()) {
        position = positionLastTick + currentMovement[0] * ((tickmod + 1) / AbstractMeasures::framesPerTick);
        return;
    }
    if (tickmod * 2 < AbstractMeasures::framesPerTick ) {
        position = positionLastTick + currentMovement[0] * 2 * ((tickmod + 1) / AbstractMeasures::framesPerTick);
        return;
    }
    position = positionLastTick + currentMovement[0] + currentMovement[1] * ((2 * tickmod) % unsigned(AbstractMeasures::framesPerTick) + 2) / AbstractMeasures::framesPerTick;
}

void Player::onGameTick() {
    if (!currentAction())
        currentAction = []() { return false; };
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

void Player::draw() const {
    renderWindow->draw(position, 0, playerSprite);
    // renderWindow->draw(position + VTile(1,1), 0, playerSprite);
    // renderWindow->draw(
    //    playerSprite,
    //    sf::RenderStates().transform
    //        .scale(1 / scale.x, 1 / scale.y)
    //        .translate(measures->getInnerWindowSizeTile().x * AbstractMeasures::pixelsPerTile / 2, measures->getInnerWindowSizeTile().y * AbstractMeasures::pixelsPerTile / 2)
    //        .scale(zoomScale, zoomScale)
    //        .translate(-(17.f/16) * measures->getTileSize().x, - 1.5 * measures->getTileSize().y)
    //        .translate(position.x - cameraPos.x, position.y - cameraPos.y)
    //);
}

void Player::clearActionIfNotBusy() {
    if (!isBusy)
        currentAction = []() { return true; };
}

void Player::setActionIfNotBusy(function<bool()> action) {
    if (!isBusy)
        currentAction = action;
}

void Player::subscribeToTeleport() {
    TeleportEvent::subscribe(new EventObserver<TeleportEvent>(
        [&](TeleportEvent* ev) {
            teleport(ev->pos);
        }
    ));
}

void Player::teleport(VTile position) {
    path = { position };
}

void Player::subscribeToInteractionClick() {
    InteractionClickEvent::subscribe(new EventObserver<InteractionClickEvent>(
        [&](InteractionClickEvent* ev) {
            interact(ev->f);
        }
    ));
}

void Player::interact(function<bool()> f) {
    setActionIfNotBusy(f);
}

void Player::subscribeToWalkClick() {
    WalkClickEvent::subscribe(new EventObserver<WalkClickEvent>(
        [&](WalkClickEvent* ev) {
            walk(ev->pos);
        }
    ));

}

void Player::walk(VTile pos) {
    // path = Pathfinder::pathfind(player->positionNextTick, { pos }, false, map);
    JSON json;
    json["x"] = std::to_string(pos.x);
    json["y"] = std::to_string(pos.y);
    json["id"] = std::to_string(id);
    socket->emit("walk", json);
}
