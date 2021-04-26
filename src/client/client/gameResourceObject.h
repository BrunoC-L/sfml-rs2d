#pragma once
#include "GameObject.h"
#include <stdlib.h>
#include "item.h"
#include "abstractInventory.h"
#include "serviceProvider.h"
#include "abstractPlayer.h"

class GameResourceObject : public GameObject {
public:
	GameResourceObject(std::function<void(VTile, int)> updateObjectTexture, std::unordered_map<VTile, std::vector<GameObject*>, VTileHash>& gameObjects, std::string objectData, ServiceProvider* provider);
	virtual std::vector<std::pair<std::string, std::function<bool()>>> getInteractions();
	void show();
	void showDepleted();
	void sendPlayerToCollect();
	bool collect();
	void sendPlayerToProspect(std::string oreType);
	bool prospect(std::string oreType);
	virtual std::string getName();
	bool isUp;
	unsigned size;
	unsigned minItem;
	unsigned maxItem;
	Item item;
	ServiceProvider* provider;
	AbstractPlayer* player;
	AbstractInventory* inventory;
};
