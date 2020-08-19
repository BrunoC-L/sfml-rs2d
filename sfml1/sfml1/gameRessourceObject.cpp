#include "gameRessourceObject.h"

GameRessourceObject::GameRessourceObject(function<void(VTile, int)> updateObjectTexture, unordered_map<VTile, vector<GameObject*>, VTileHash>& gameObjects, string objectData) :
	GameObject(id, updateObjectTexture) {
	vector<string> parts = split(objectData, "///");
	string itemPart = parts[1];
	item = Item::fromString(itemPart, "//");
	vector<string> objectPart = split(parts[0], "//");
	name = objectPart[0];
	id = stoi(objectPart[1]);
	size = stoi(objectPart[2]);
	VTile positionNW(stoi(objectPart[3]), stoi(objectPart[4]));
	unsigned firstTextureIndex = stoi(objectPart[5]);
	minItem = stoi(objectPart[6]);
	maxItem = stoi(objectPart[7]);
	for (auto dx = 0; dx < size; ++dx)
		for (auto dy = 0; dy < size; ++dy) {
			positions.push_back(positionNW + VTile(dx, dy));
			gameObjects[positions.back()] = { this };
			groundObjects.push_back(GroundObject(id, positions.back(), firstTextureIndex + dx + size * dy, updateObjectTexture, true));
			groundObjects.push_back(GroundObject(id, positions.back(), firstTextureIndex + dx + size * dy + size * size, updateObjectTexture, false));
		}
	isUp = true;
	string examineOptions = objectPart[8];
	auto examines = split(examineOptions, "|");
	string collectOption  = objectPart[9];
	interactions = {
		make_pair(
			collectOption,
			[this]() { sendPlayerToStartCollecting(); return true; }
		),
		make_pair(
			"Examine",
			[this, examines]() { cout << examines[isUp ? 0 : 1] << endl; return false; }
		),
	};
}

void GameRessourceObject::show() {
	isUp = true;
	for (auto i = 0; i < 2 * size * size; i += 2)
		groundObjects[i].show();
}

void GameRessourceObject::showDepleted() {
	isUp = false;
	for (auto i = 1; i < 2 * size * size; i += 2)
		groundObjects[i].show();
}

void GameRessourceObject::sendPlayerToStartCollecting() {
	Player& player = Player::getInstance();
	player.path = Pathfinder::pathfind(player.positionNextTick, positions, true);
	unsigned tick = GameTick::get();
	player.currentAction = [&, tick]() {
		auto nextToTree = MovingPredicate::getNextTo(positions);
		if (MovingPredicate::tileIsInVector(player.positionNextTick, nextToTree)) {
			player.currentAction = [&]() { return collect(); };
			return true;
		}
		return player.path.size() && MovingPredicate::tileIsInVector(player.path.back(), nextToTree);
	};
}

bool GameRessourceObject::collect() {
	auto nextToTree = MovingPredicate::getNextTo(positions);
	Player& player = Player::getInstance();
	if (!MovingPredicate::tileIsInVector(player.position, nextToTree))
		return false;
	if (!player.inventory.space() && !item.stackable || item.stackable && player.inventory.has(item, 1))
		return false;
	if (rand() % 100 < 10) {
		if (maxItem - minItem)
			player.inventory.add(item, rand() % (maxItem - minItem) + minItem);
		else
			player.inventory.add(item, minItem);
		if (rand() % 100 < 10) {
			showDepleted();
			TaskManager::getInstance().scheduleTaskInTicks([&]() { this->show(); return false; }, rand() % 20);
			return false;
		}
	}
	return true;
}

vector<pair<string, function<bool()>>> GameRessourceObject::getInteractions() {
	if (isUp)
		return interactions;
	return vector<pair<string, function<bool()>>>(interactions.begin() + 1, interactions.end());
}

string GameRessourceObject::getName() {
	return split(name, "|")[isUp ? 0 : 1];
}
