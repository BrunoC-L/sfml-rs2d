#pragma once
#include "abstractServices.h"
#include "../../common/common/abstractServiceProvider.h"

class Service {
public:
	AbstractServiceProvider* provider;
	AbstractCamera* camera;
	AbstractChat* chat;
	AbstractInventory* inventory;
	AbstractMap* map;
	AbstractMeasures* measures;
	AbstractPlayer* player;
	AbstractSocket* socket;
	AbstractGameDataService* gameData;

	Service(AbstractServiceProvider* provider) : provider(provider) { }

	void acquire() {
		camera = (AbstractCamera*)provider->get("Camera");
		chat = (AbstractChat*)provider->get("Chat");
		inventory = (AbstractInventory*)provider->get("Inventory");
		map = (AbstractMap*)provider->get("Map");
		measures = (AbstractMeasures*)provider->get("Measures");
		player = (AbstractPlayer*)provider->get("Player");
		socket = (AbstractSocket*)provider->get("Socket");
		gameData = (AbstractGameDataService*)provider->get("GameData");
	}
};
