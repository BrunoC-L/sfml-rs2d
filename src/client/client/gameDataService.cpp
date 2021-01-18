#include "gameDataService.h"
#include "constants.h"
#include "login.h"

GameDataService::GameDataService(
    AbstractServiceProvider* provider,
    GameTickProgress* tracker,
    GameDataStorage* storage
) : Service(provider),
    AbstractGameDataService(tracker, storage) {
	provider->set("GameData", this);
    LoginEvent::subscribe(
        new EventObserver<LoginEvent>(
            [&](LoginEvent* ev) {
                auto& data = ev->json;
                int id = data.asInt();
                gameData->storage->playerId = id;
                userIsLoggedIn = true;
            }
        )
    );
}

std::vector<VTile> GameDataService::getPlayerPositions() {
    double tickFraction = tracker->getTickFraction();
    return storage->getGameData(tickFraction).playerPositions;
}

void GameDataService::init() {
	acquire();
    socket->on("GameTick",
        [&](JSON json) {
            storage->onGameTick(json);
            tracker->onGameTick();
        }
    );
}
