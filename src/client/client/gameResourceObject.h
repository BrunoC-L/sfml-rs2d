#pragma once
#include "GameObject.h"
#include <stdlib.h>
#include "item.h"
#include "abstractInventory.h"
#include "abstractServiceProvider.h"
#include "abstractPlayer.h"

class GameResourceObject : public GameObject {
public:
	GameResourceObject(function<void(VTile, int)> updateObjectTexture, unordered_map<VTile, vector<GameObject*>, VTileHash>& gameObjects, string objectData, AbstractServiceProvider* provider);
	virtual vector<pair<string, function<bool()>>> getInteractions();
	void show();
	void showDepleted();
	void sendPlayerToCollect();
	bool collect();
	void sendPlayerToProspect(string oreType);
	bool prospect(string oreType);
	virtual string getName();
	bool isUp;
	unsigned size;
	unsigned minItem;
	unsigned maxItem;
	Item item;
	AbstractServiceProvider* provider;
	AbstractPlayer* player;
	AbstractInventory* inventory;
};
