#include "player.h"
#include "login.h"

Player::Player(AbstractServiceProvider* provider): Service(provider) {
    provider->set("Player", this);
    position = VTile();
    LoginEvent::subscribe(
        new EventObserver<LoginEvent>(
            [&](LoginEvent* ev) {
                auto& data = ev->json;
                int id = data.asInt();
                player->id = id;
            }
        )
    );
}

void Player::init() {
    acquire();
    VTile lumbridge(18 * AbstractMeasures::TilesPerChunk + 20, 13 * AbstractMeasures::TilesPerChunk + 37, 0);
    position = lumbridge;
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
