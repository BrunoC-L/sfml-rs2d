#include "tile.h"
#include "pathfinder.h"

Tile::Tile(int x, int y, int borders, vector<GroundItem> groundItems, vector<GameObject*> gameObjects, vector<NPC> NPCs, function<void()> callback)
	: position(x, y), borders(borders), groundItems(groundItems), NPCs(NPCs), callback(callback) {
	if (gameObjects.size()) {
		for (auto gameObject : gameObjects)
			for (auto groundObject : gameObject->groundObjects)
				if (position == groundObject.position) {
					this->gameObjects.push_back(gameObject);
					groundObjects.push_back(groundObject);
				}
	}
	walkable = borders != 0b1111;
}

bool Tile::canMoveFrom(Tile from) {
	VTile delta = from.position - position;
	const int EAST = 0b1000, NORTH = 0b0100, WEST = 0b0010, SOUTH = 0b0001;
	const int mycase = 10 * delta.x + delta.y;
	// borders: 0b1101 ->  east, north, !west, south
	// -1, -1     0, -1     1, -1		|	 -11,  -1,   9
	// -1,  0     0,  0     1,  0		|	 -10,   0,  10
	// -1,  1     0,  1     1,  1		|	  -9,   1,  11
	switch (mycase) {
		case   0: return true;
		case -11: return !(borders & (NORTH | WEST) || from.borders & (SOUTH | EAST));
		case -10: return !(borders & WEST || from.borders & EAST);
		case  -9: return !(borders & (SOUTH | WEST) || from.borders & (NORTH | EAST));
		case  -1: return !(borders & NORTH || from.borders & SOUTH);
		case  11: 
		case  10:
		case   9:
		case   1: return from.canMoveFrom(*this);
		default:
			return false;
	}
}

function<bool()> Tile::onLeftClick(MouseLeftClickEvent event) {
	auto& player = Player::getInstance();
	RightClickInterface& rightClickInterface = RightClickInterface::getInstance();
	bool redClick = false;
	for (int i = 0; !redClick && i < groundObjects.size(); ++i) {
		auto groundObject = groundObjects[i];
		auto gameObject = gameObjects[i];
		string name = gameObject->getName();
		if (groundObject.isVisible && gameObject->hasRedClickAction && groundObject.isOverObject(1, 1)) {
			auto interactions = gameObject->getInteractions();
			if (interactions.size())
				if (interactions[0].first != "Examine") {
					player.currentAction = interactions[0].second;
					redClick = true;
				}
		}
	}
	if (!redClick)
		player.path = Pathfinder::pathfind(player.positionNextTick, { position }, false);
	return []() { return false; };
}

function<bool()> Tile::onRightClick(MouseRightClickEvent event) {
	auto& player = Player::getInstance();
	RightClickInterface& rightClickInterface = RightClickInterface::getInstance();
	rightClickInterface.active = true;
	rightClickInterface.resetText();
	for (int i = 0; i < groundObjects.size(); ++i) {
		auto groundObject = groundObjects[i];
		auto gameObject = gameObjects[i];
		string name = gameObject->getName();
		if (groundObject.isVisible && gameObject->hasRedClickAction && groundObject.isOverObject(1, 1))
			for (auto interactions : gameObject->getInteractions()) {
				rightClickInterface.setText(gameObject->getName() + '\t' + interactions.first);
			}
	}
	return []() { return false; };
}

function<bool()> Tile::onMiddleClick(MouseMiddleClickEvent event) {
	auto& player = Player::getInstance();
	player.path = { position };
	return []() { return false; };
}

Tile::~Tile() {
	/*for (const auto& obj : gameObjects)
		delete obj;*/
}

function<bool()> Tile::onMove(MouseMoveEvent event) {
	return []() { return false; };
}
