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
