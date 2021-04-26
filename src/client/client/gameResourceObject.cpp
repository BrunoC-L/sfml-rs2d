#include "gameResourceObject.h"

GameResourceObject::GameResourceObject(
	std::function<void(VTile, int)> updateObjectTexture,
	std::unordered_map<VTile, std::vector<GameObject*>,
	VTileHash>& gameObjects, std::string objectData,
	ServiceProvider* provider
) : GameObject(id, updateObjectTexture) {
	this->provider = provider;
	std::vector<std::string> parts = split(objectData, "///");
	std::string itemPart = parts[1];
	item = Item::fromString(itemPart, "//");
	std::vector<std::string> objectPart = split(parts[0], "//");
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
	std::string collectOption = objectPart[9];
	std::string examineOptions = objectPart[8];
	auto examines = split(examineOptions, "|");
	interactions = {
		make_pair(
			collectOption,
			[this]() { sendPlayerToCollect(); return true; }
		),
		std::make_pair(
			"Examine",
			[this, examines]() { /*cout << examines[isUp ? 0 : 1] << endl;*/ return false; }
		),
	};
	if (objectPart.size() > 10) {
		auto parts = split(objectPart[10], "|");
		std::string option = parts[0];
		if (option == "Prospect") {
			std::string whatsInside = parts[1];
			interactions.insert(interactions.begin() + 1, std::make_pair(
				"Prospect",
				[this, whatsInside]() { sendPlayerToProspect(whatsInside); return true; }
			));
		}
	}
}

void GameResourceObject::show() {
	isUp = true;
	for (auto i = 0; i < 2 * size * size; i += 2)
		groundObjects[i].show();
}

void GameResourceObject::showDepleted() {
	isUp = false;
	for (auto i = 1; i < 2 * size * size; i += 2)
		groundObjects[i].show();
}

void GameResourceObject::sendPlayerToCollect() {
	//player->path = Pathfinder::pathfind(player->positionNextTick, positions, true, (AbstractMap*)provider->get("Map"));
	//player->setActionIfNotBusy([&]() {
	//	auto nextToResource = MovingPredicate::getNextTo(positions);
	//	if (MovingPredicate::tileIsInVector(player->positionNextTick, nextToResource)) {
	//		player->setActionIfNotBusy([&]() { return collect(); });
	//		return true;
	//	}
	//	return player->path.size() && MovingPredicate::tileIsInVector(player->path.back(), nextToResource);
	//});
}

bool GameResourceObject::collect() {
	//auto nextToResource = MovingPredicate::getNextTo(positions);
	//if (!MovingPredicate::tileIsInVector(player->position, nextToResource))
	//	return false;
	//if (!inventory->space() && !item.stackable || item.stackable && inventory->has(item, 1))
	//	return false;
	//if (rand() % 100 >= 10)
	//	return true;
	//if (maxItem - minItem)
	//	inventory->add(item, rand() % (maxItem - minItem) + minItem);
	//else
	//	inventory->add(item, minItem);
	//if (rand() % 100 < 10) {
	//	showDepleted();
	//	taskManager->scheduleTaskInTicks([&]() { this->show(); return false; }, rand() % 20);
	//	return false;
	//}
	return true;
}

void GameResourceObject::sendPlayerToProspect(std::string oreType) {
	//player->path = Pathfinder::pathfind(player->positionNextTick, positions, true, (AbstractMap*)provider->get("Map"));
	//player->setActionIfNotBusy([&, oreType]() {
	//	auto nextToResource = MovingPredicate::getNextTo(positions);
	//	if (MovingPredicate::tileIsInVector(player->positionNextTick, nextToResource)) {
	//		player->setActionIfNotBusy([&, oreType]() { return prospect(oreType); });
	//		return true;
	//	}
	//	return player->path.size() && MovingPredicate::tileIsInVector(player->path.back(), nextToResource);
	//	});
}

bool GameResourceObject::prospect(std::string oreType) {
	//auto nextToResource = MovingPredicate::getNextTo(positions);
	//if (!MovingPredicate::tileIsInVector(player->position, nextToResource))
	//	return false;
	//if (rand() % 100 >= 10)
	//	return true;
	//if (rand() % 100 >= 70)
	//	cout << "You fail to identify the ore :(" << endl;
	//else
	//	cout << oreType << endl;
	return false;
}

std::vector<std::pair<std::string, std::function<bool()>>> GameResourceObject::getInteractions() {
	if (isUp)
		return interactions;
	return std::vector<std::pair<std::string, std::function<bool()>>>(interactions.begin() + 1, interactions.end());
}

std::string GameResourceObject::getName() {
	return split(name, "|")[isUp ? 0 : 1];
}
