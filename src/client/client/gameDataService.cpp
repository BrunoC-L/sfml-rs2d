#include "gameDataService.h"
#include "constants.h"
#include "login.h"

GameDataService::GameDataService(AbstractServiceProvider* provider, GameTickProgress* tracker) : Service(provider), tracker(tracker) {
    provider->set("GameData", this);

    LoginEvent::subscribe(
        new EventObserver<LoginEvent>(
            [&](LoginEvent* ev) {
                auto& data = ev->json;
                int id = data.asInt();
                player->id = id;
                loggedIn = true;
            }
        )
    );
}

void GameDataService::init() {
    acquire();
    socket->on("positions", [&](JSON& json) {
        storePositions(json);
        tracker->onGameTick();
    });
}

bool GameDataService::userIsLoggedIn() {
    return loggedIn;
}

std::vector<playerIdAndPosition> GameDataService::getPlayerPositions() {
    return playerPositions.getPlayerPositions(tracker->getTickFraction());
}

void GameDataService::storePositions(JSON& json) {
    playerPositions.update(json);
}
