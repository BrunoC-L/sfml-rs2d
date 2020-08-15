#include "gameRessourceObject.h"

GameRessourceObject::GameRessourceObject(int id, VTile position, unsigned size, function<void(VTile, int)> updateObjectTexture, string itemData) :
	GameObject(id, updateObjectTexture), size(size) {
	item = Item::fromString(itemData);
	for (auto dx = 0; dx < size; ++dx)
		for (auto dy = 0; dy < size; ++dy) {
			positions.push_back(position + VTile(dx, dy));
			groundObjects.push_back(GroundObject(id, positions.back(), dx + 2 * dy + 1, updateObjectTexture, true));
			groundObjects.push_back(GroundObject(id, positions.back(), dx + 2 * dy + 5, updateObjectTexture, false));
		}
	isUp = true;
	interactions = {
		make_pair(
			"Chop Down",
			[this]() { sendPlayerToStartCollecting(); return true; }
		),
		make_pair(
			"Examine",
			[]() { cout << "A TREE!@!!!!\n"; return true; }
		),
	};
}

void GameRessourceObject::show() {
	isUp = true;
	for (auto i = 0; i < 8; i += 2)
		groundObjects[i].show();
}

void GameRessourceObject::showDepleted() {
	isUp = false;
	for (auto i = 1; i < 8; i += 2)
		groundObjects[i].show();
}

void GameRessourceObject::sendPlayerToStartCollecting() {
	Player& player = Player::getInstance();
	player.path = Pathfinder::pathfind(player.positionNextTick, positions, true);
	unsigned tick = GameTick::get();
	TaskManager::getInstance().scheduleTaskInTicks([&, tick]() {
		auto nextToTree = MovingPredicate::getNextTo(positions);
		if (MovingPredicate::tileIsInVector(player.positionNextTick, nextToTree)) {
			TaskManager::getInstance().scheduleTaskInTicks([&]() { collect(); return isUp; }, 1);
			return false;
		}
		return player.path.size() && MovingPredicate::tileIsInVector(player.path.back(), nextToTree);
		}, (player.path.size() + 1) / 2);
}

bool GameRessourceObject::collect() {
	auto nextToTree = MovingPredicate::getNextTo(positions);
	Player& player = Player::getInstance();
	if (!MovingPredicate::tileIsInVector(player.position, nextToTree))
		return true;
	if (!player.inventory.space())
		return false;
	if (rand() % 100 > 90) {
		player.inventory.add(item);
		TaskManager::getInstance().scheduleTaskInTicks([&]() { this->show(); return false; }, rand() % 20);
		if (rand() % 100 > 90) {
			showDepleted();
			return true;
		}
	}
	return false;
}

vector<pair<string, function<bool()>>> GameRessourceObject::getInteractions() {
	vector<pair<string, function<bool()>>> availableInteractions = {};
	if (isUp)
		availableInteractions.push_back(interactions[0]);
	else
		availableInteractions.push_back(interactions[2]);
	availableInteractions.push_back(interactions[1]);
	return availableInteractions;
}
