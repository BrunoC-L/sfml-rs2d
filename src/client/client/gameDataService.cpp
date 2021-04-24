#include "gameDataService.h"
#include "constants.h"
#include "login.h"

GameDataService::GameDataService(ServiceProvider* provider, GameTickProgress* tracker) : Service(provider), tracker(tracker) {
    provider->set("GameData", this);
}

void GameDataService::init() {
    acquire();
    socket->on("positions", [&](JSON& json) {
        storePositions(json);
        tracker->onGameTick();
		//std::cout << "GameTick " << tracker->getTick() << std::endl;
    });

	LoginEvent::subscribe(
		new EventObserver<LoginEvent>(
			[&](LoginEvent* ev) {
				loggedIn = true;
				// Temporal link with the player subscription...
				// maybe add a new event called when the player receives login
				// the other solution is for the gamedataservice to own the player
				playerPositions = std::make_unique<PlayerPositions>(player);
			}
		)
	);

	LogoutEvent::subscribe(
		new EventObserver<LogoutEvent>(
			[&](LogoutEvent* ev) {
				loggedIn = false;
				playerPositions.reset();
			}
		)
	);
}

const bool& GameDataService::userIsLoggedIn() {
    return loggedIn;
}

std::vector<playerIdAndPosition> GameDataService::getPlayerPositions() {
	if (playerPositions == nullptr)
		throw std::exception("Asking for positions before login");
    return playerPositions->getPlayerPositions(tracker->getTickFraction());
}

void GameDataService::storePositions(JSON& json) {
	if (playerPositions == nullptr)
		throw std::exception("Giving positions before login");
    playerPositions->update(json);
}
