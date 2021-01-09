#pragma once
#include "abstractServices.h"
#include "../../common/abstractServiceProvider.h"

class Service {
protected:
	AbstractServiceProvider* provider;
	AbstractCamera* camera;
	AbstractChat* chat;
	AbstractInventory* inventory;
	AbstractMap* map;
	AbstractMeasures* measures;
	AbstractPlayer* player;
	AbstractRenderWindow* renderWindow;
	AbstractTaskManager* taskManager;
	AbstractSocket* socket;
	AbstractGameDataService* gameData;
public:
	Service(AbstractServiceProvider* provider) : provider(provider) { }

	void acquire() {
		camera = (AbstractCamera*)provider->get("Camera");
		chat = (AbstractChat*)provider->get("Chat");
		inventory = (AbstractInventory*)provider->get("Inventory");
		map = (AbstractMap*)provider->get("Map");
		measures = (AbstractMeasures*)provider->get("Measures");
		player = (AbstractPlayer*)provider->get("Player");
		renderWindow = (AbstractRenderWindow*)provider->get("RenderWindow");
		taskManager = (AbstractTaskManager*)provider->get("TaskManager");
		socket = (AbstractSocket*)provider->get("Socket");
		gameData = (AbstractGameDataService*)provider->get("GameData");
	}
};
