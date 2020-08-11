#pragma once
#include "GameObject.h"
#include "taskManager.h"
#include <stdlib.h>

class Tree : public GameObject {
public:
	Tree(int id, int* tilemapPtr, VTile position, function<void(VTile, int)> updateObjectTexture);
	virtual vector<pair<string, function<bool()>>> getInteractions();
	void showTree();
	void showStump();
	void chop();
	const VTile position;
	bool treeIsUp;
};
