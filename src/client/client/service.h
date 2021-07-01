#pragma once
#include "abstractServices.h"
#include "serviceProvider.h"
#include "services.h"

class Service {
public:
	ServiceProvider* provider;
	AbstractCamera* camera;
	AbstractChat* chat;
	AbstractMap* map;
	AbstractMeasures* measures;
	AbstractPlayer* player;
	AbstractSocket* socket;
	AbstractGameDataService* gameData;

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
