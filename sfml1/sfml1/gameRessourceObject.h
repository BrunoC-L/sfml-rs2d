#pragma once
#include "GameObject.h"
#include "taskManager.h"
#include <stdlib.h>
#include "debug.h"
#include "pathfinder.h"

class GameRessourceObject : public GameObject {
public:
	GameRessourceObject(int id, VTile position, unsigned size, function<void(VTile, int)> updateObjectTexture, string itemData);
	virtual vector<pair<string, function<bool()>>> getInteractions();
	void show();
	void showDepleted();
	bool collect();
	void sendPlayerToStartCollecting();
	bool isUp;
	unsigned size;
	Item item;
};
