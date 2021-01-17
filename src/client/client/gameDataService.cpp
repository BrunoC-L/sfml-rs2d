#include "gameDataService.h"
#include "constants.h"

GameDataService::GameDataService(
    AbstractServiceProvider* provider,
    GameTickProgress* tracker,
    GameDataStorage* data
) : Service(provider),
    AbstractGameDataService(tracker, data) {
	provider->set("GameData", this);
}

std::vector<VTile> GameDataService::getPlayerPositions() {
    double tickFraction = tracker->getTickFraction();
    return data->getGameData(tickFraction).playerPositions;
}

void GameDataService::init() {
	acquire();
    socket->on("GameTick",
        [&](JSON json) {
            data->onGameTick(json);
            tracker->onGameTick();
        }
    );
}
