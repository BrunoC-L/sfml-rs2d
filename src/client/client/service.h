#pragma once
#include "abstractServices.h"
#include "serviceProvider.h"
#include "services.h"

class Service {
public:
	ServiceProvider* provider = nullptr;
	AbstractCamera* camera = nullptr;
	AbstractChat* chat = nullptr;
	AbstractMap* map = nullptr;
	AbstractMeasures* measures = nullptr;
	AbstractPlayer* player = nullptr;
	AbstractSocket* socket = nullptr;
	AbstractGameDataService* gameData = nullptr;

	Service(ServiceProvider* provider) : provider(provider) { }

	void acquire() {
		camera = (AbstractCamera*)provider->get(CAMERA);
		chat = (AbstractChat*)provider->get(CHAT);
		map = (AbstractMap*)provider->get(MAP);
		measures = (AbstractMeasures*)provider->get(MEASURES);
		player = (AbstractPlayer*)provider->get(PLAYER);
		socket = (AbstractSocket*)provider->get(SOCKET);
		gameData = (AbstractGameDataService*)provider->get(GAMEDATA);
	}
};
