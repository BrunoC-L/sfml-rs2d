#include "gameDataService.h"

GameDataService::GameDataService(AbstractServiceProvider* provider) : Service(provider) {
	provider->set("GameData", this);
}

void GameDataService::init() {
	acquire();
    socket->on("positions",
        [&](JSON json) {
            auto _positions = json.children;
            for (auto data : _positions) {
                int otherid = data["id"].asInt();
                int x = data["x"].asDouble();
                int y = data["y"].asDouble();
                if (otherid == player->id) {
                    player->position.x = x;
                    player->position.y = y;
                } else {
                    playerPositions[otherid] = VTile(x, y);
                    this->map->shouldUpdate = true;
                }
            }
        }
    );
}
