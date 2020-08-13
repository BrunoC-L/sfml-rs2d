#include "tree.h"

Tree::Tree(int id, int* tilemapPtr, VTile position, function<void(VTile, int)> updateObjectTexture) : GameObject(id, tilemapPtr, updateObjectTexture) {
	for (auto dx = 0; dx < 2; ++dx)
		for (auto dy = 0; dy < 2; ++dy) {
			positions.push_back(position + VTile(dx, dy));
			groundObjects.push_back(GroundObject(id, positions.back(), dx + 2 * dy + 1, updateObjectTexture, true));
			groundObjects.push_back(GroundObject(id, positions.back(), dx + 2 * dy + 5, updateObjectTexture, false));
		}
	treeIsUp = true;
	interactions = {
		make_pair(
			"Chop Down",
			[this]() { sendPlayerToStartCuttingTree(); return true; }
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
}

void Tree::showTree() {
	treeIsUp = true;
	for (auto i = 0; i < 8; i += 2)
		groundObjects[i].show();
}

void Tree::showStump() {
	treeIsUp = false;
	for (auto i = 1; i < 8; i += 2)
		groundObjects[i].show();
}

void Tree::sendPlayerToStartCuttingTree() {
	Player& player = Player::getInstance();
	player.path = Pathfinder::pathfind(player.positionNextTick, positions, true);
	unsigned tick = GameTick::get();
	TaskManager::getInstance().scheduleTaskInTicks([&, tick]() {
		auto nextToTree = MovingPredicate::getNextTo(positions);
		if (MovingPredicate::tileIsInVector(player.positionNextTick, nextToTree)) {
			TaskManager::getInstance().scheduleTaskInTicks([&]() {return !chop(); }, 1);
			return false;
		}
		return player.path.size() && MovingPredicate::tileIsInVector(player.path.back(), nextToTree);
	}, (player.path.size() + 1) / 2);
}

bool Tree::chop() {
	if (rand() % 100 > 90) {
		showStump();
		TaskManager::getInstance().scheduleTaskInTicks([&]() { this->showTree(); return false; }, rand() % 20);
		return true;
	}
	return false;
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