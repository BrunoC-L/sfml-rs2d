#pragma once
#include "GameObject.h"
#include "taskManager.h"
#include <stdlib.h>
#include "debug.h"
#include "pathfinder.h"

class GameRessourceObject : public GameObject {
public:
	GameRessourceObject(function<void(VTile, int)> updateObjectTexture, unordered_map<VTile, vector<GameObject*>, VTileHash>& gameObjects, string objectData);
	virtual vector<pair<string, function<bool()>>> getInteractions();
	void show();
	void showDepleted();
	bool collect();
	void sendPlayerToStartCollecting();
	virtual string getName();
	bool isUp;
	unsigned size;
	unsigned minItem;
	unsigned maxItem;
	Item item;
};
