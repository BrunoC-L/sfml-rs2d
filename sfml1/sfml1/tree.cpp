#include "tree.h"

Tree::Tree(int id, int* tilemapPtr, VTile position, function<void(VTile, int)> updateObjectTexture) : GameObject(id, tilemapPtr, updateObjectTexture), position(position) {
	treeIsUp = true;
	interactions = {
		make_pair(
			"Chop Down",
			[this]() { print(Player::getInstance().position); chop(); return true; }
		),
		make_pair(
			"Examine",
			[]() { cout << "A TREE!@!!!!\n"; return true; }
		),
		make_pair(
			"Revive",
			[this]() { cout << "revive\n"; showTree(); return true; }
		),
	};

	groundObjects = {
		GroundObject(id, position + VTile(0, 0), 1, updateObjectTexture, true),
		GroundObject(id, position + VTile(1, 0), 2, updateObjectTexture, true),
		GroundObject(id, position + VTile(0, 1), 3, updateObjectTexture, true),
		GroundObject(id, position + VTile(1, 1), 4, updateObjectTexture, true),
		GroundObject(id, position + VTile(0, 0), 5, updateObjectTexture, false),
		GroundObject(id, position + VTile(1, 0), 6, updateObjectTexture, false),
		GroundObject(id, position + VTile(0, 1), 7, updateObjectTexture, false),
		GroundObject(id, position + VTile(1, 1), 8, updateObjectTexture, false),
	};
}

void Tree::showTree() {
	treeIsUp = true;
	for (auto i = 0; i < 4; ++i)
		groundObjects[i].show();
}

void Tree::showStump() {
	treeIsUp = false;
	for (auto i = 4; i < 8; ++i)
		groundObjects[i].show();
}

void Tree::chop() {
	showStump();
	TaskManager::getInstance().scheduleTaskInTicks([&]() { this->showTree(); }, rand() % 20);
}

vector<pair<string, function<bool()>>> Tree::getInteractions() {
	vector<pair<string, function<bool()>>> availableInteractions = {};
	if (treeIsUp)
		availableInteractions.push_back(interactions[0]);
	else
		availableInteractions.push_back(interactions[2]);
	availableInteractions.push_back(interactions[1]);
	return availableInteractions;
}