#include "gameDataService.h"
#include "constants.h"

GameDataService::GameDataService(
    AbstractServiceProvider* provider,
    GameTickProgress* tracker,
    GameDataStorage* data
) : Service(provider),
    tracker(tracker),
    data(data) {
	provider->set("GameData", this);
}

std::vector<VTile> GameDataService::getPlayerPositions() {
    double tickFraction = tracker->getTickFraction();
    return data->getGameData(tickFraction).playerPositions;
}

void GameDataService::updatePlayerPosition() {
    //auto pair = playerPositions[player->id];
    //if (pair.second == VTile())
    //    pair.second = player->position;
    //double tickFraction = 1;
    //if (pair.first == VTile())
    //    player->position = pair.second;
    //else
    //    player->position = pair.first * (1 - tickFraction) + pair.second * tickFraction;
}

void GameDataService::init() {
	acquire();
    socket->on("GameTick",
        [&](JSON json) {
            data->onGameTick(json);
        }
    );
}
