#include "player.h"

Player::Player(AbstractServiceProvider* provider): Service(provider) {
    provider->set("Player", this);
    position = VTile();
}

void Player::init() {
    acquire();
    VTile lumbridge(18 * AbstractMeasures::TilesPerChunk + 20, 13 * AbstractMeasures::TilesPerChunk + 37, 0);
    position = lumbridge;
    subscribeToTeleport();
    subscribeToInteractionClick();
    subscribeToWalkClick();

    socket->on("hello",
        [&](JSON data) {
            player->id = data.asInt();
        }
    );
}

void Player::update(unsigned tickmod) {
    //if (currentMovement[0] == VTile())
    //    return;
    //if (currentMovement[1] == VTile()) {
    //    position = positionLastTick + currentMovement[0] * ((tickmod + 1) / AbstractMeasures::framesPerTick);
    //    return;
    //}
    //if (tickmod * 2 < AbstractMeasures::framesPerTick ) {
    //    position = positionLastTick + currentMovement[0] * 2 * ((tickmod + 1) / AbstractMeasures::framesPerTick);
    //    return;
    //}
    //position = positionLastTick + currentMovement[0] + currentMovement[1] * ((2 * tickmod) % unsigned(AbstractMeasures::framesPerTick) + 2) / AbstractMeasures::framesPerTick;
}

void Player::onGameTick() {
    //if (!currentAction())
    //    currentAction = []() { return false; };
    //positionLastTick = position;
    //if (path.size()) {
    //    currentMovement[0] = path[0] - position;
    //    path.erase(path.begin());
    //    if (path.size()) {
    //        currentMovement[1] = path[0] - (position + currentMovement[0]);
    //        path.erase(path.begin());
    //    }
    //    else
    //        currentMovement[1] = VTile();
    //}
    //else {
    //    currentMovement[0] = VTile();
    //    currentMovement[1] = VTile();
    //}
    //positionNextTick = positionLastTick + currentMovement[0] + currentMovement[1];
}

void Player::subscribeToTeleport() {
    TeleportEvent::subscribe(new EventObserver<TeleportEvent>(
        [&](TeleportEvent* ev) {
            teleport(ev->pos);
        }
    ));
}

void Player::teleport(VTile position) {
    //path = { position };
}

void Player::subscribeToInteractionClick() {
    InteractionClickEvent::subscribe(new EventObserver<InteractionClickEvent>(
        [&](InteractionClickEvent* ev) {
        }
    ));
}

void Player::subscribeToWalkClick() {
    WalkClickEvent::subscribe(new EventObserver<WalkClickEvent>(
        [&](WalkClickEvent* ev) {
            walk(ev->pos);
        }
    ));

}

void Player::walk(VTile pos) {
    JSON json;
    json["x"] = std::to_string(pos.x);
    json["y"] = std::to_string(pos.y);
    json["id"] = std::to_string(id);
    socket->emit("walk", json);
}
