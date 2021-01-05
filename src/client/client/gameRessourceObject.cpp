#include "gameRessourceObject.h"

GameRessourceObject::GameRessourceObject(
	function<void(VTile, int)> updateObjectTexture,
	unordered_map<VTile, vector<GameObject*>,
	VTileHash>& gameObjects, string objectData,
	AbstractServiceProvider* provider
) : GameObject(id, updateObjectTexture) {
	this->provider = provider;
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
	string collectOption = objectPart[9];
	string examineOptions = objectPart[8];
	auto examines = split(examineOptions, "|");
	interactions = {
		make_pair(
			collectOption,
			[this]() { sendPlayerToCollect(); return true; }
		),
		make_pair(
			"Examine",
			[this, examines]() { cout << examines[isUp ? 0 : 1] << endl; return false; }
		),
	};
	if (objectPart.size() > 10) {
		auto parts = split(objectPart[10], "|");
		string option = parts[0];
		if (option == "Prospect") {
			string whatsInside = parts[1];
			interactions.insert(interactions.begin() + 1, make_pair(
				"Prospect",
				[this, whatsInside]() { sendPlayerToProspect(whatsInside); return true; }
			));
		}
	}
	player = (AbstractPlayer*)provider->get("Player");
	inventory = (AbstractInventory*)provider->get("Inventory");
	taskManager = (AbstractTaskManager*)provider->get("TaskManager");
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

void GameRessourceObject::sendPlayerToCollect() {
	player->path = Pathfinder::pathfind(player->positionNextTick, positions, true, (AbstractMap*)provider->get("Map"));
	player->setActionIfNotBusy([&]() {
		auto nextToRessource = MovingPredicate::getNextTo(positions);
		if (MovingPredicate::tileIsInVector(player->positionNextTick, nextToRessource)) {
			player->setActionIfNotBusy([&]() { return collect(); });
			return true;
		}
		return player->path.size() && MovingPredicate::tileIsInVector(player->path.back(), nextToRessource);
	});
}

bool GameRessourceObject::collect() {
	auto nextToRessource = MovingPredicate::getNextTo(positions);
	if (!MovingPredicate::tileIsInVector(player->position, nextToRessource))
		return false;
	if (!inventory->space() && !item.stackable || item.stackable && inventory->has(item, 1))
		return false;
	if (rand() % 100 >= 10)
		return true;
	if (maxItem - minItem)
		inventory->add(item, rand() % (maxItem - minItem) + minItem);
	else
		inventory->add(item, minItem);
	if (rand() % 100 < 10) {
		showDepleted();
		taskManager->scheduleTaskInTicks([&]() { this->show(); return false; }, rand() % 20);
		return false;
	}
	return true;
}

void GameRessourceObject::sendPlayerToProspect(string oreType) {
	player->path = Pathfinder::pathfind(player->positionNextTick, positions, true, (AbstractMap*)provider->get("Map"));
	player->setActionIfNotBusy([&, oreType]() {
		auto nextToRessource = MovingPredicate::getNextTo(positions);
		if (MovingPredicate::tileIsInVector(player->positionNextTick, nextToRessource)) {
			player->setActionIfNotBusy([&, oreType]() { return prospect(oreType); });
			return true;
		}
		return player->path.size() && MovingPredicate::tileIsInVector(player->path.back(), nextToRessource);
		});
}

bool GameRessourceObject::prospect(string oreType) {
	auto nextToRessource = MovingPredicate::getNextTo(positions);
	if (!MovingPredicate::tileIsInVector(player->position, nextToRessource))
		return false;
	if (rand() % 100 >= 10)
		return true;
	if (rand() % 100 >= 70)
		cout << "You fail to identify the ore :(" << endl;
	else
		cout << oreType << endl;
	return false;
}

vector<pair<string, function<bool()>>> GameRessourceObject::getInteractions() {
	if (isUp)
		return interactions;
	return vector<pair<string, function<bool()>>>(interactions.begin() + 1, interactions.end());
}

string GameRessourceObject::getName() {
	return split(name, "|")[isUp ? 0 : 1];
}
