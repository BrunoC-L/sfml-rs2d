#pragma once
#include "GameObject.h"
#include "taskManager.h"
#include <stdlib.h>
#include "debug.h"
#include "pathfinder.h"

class Tree : public GameObject {
public:
	Tree(int id, int* tilemapPtr, VTile position, function<void(VTile, int)> updateObjectTexture);
	virtual vector<pair<string, function<bool()>>> getInteractions();
	void showTree();
	void showStump();
	bool chop();
	void sendPlayerToStartCuttingTree();
	bool treeIsUp;
};
