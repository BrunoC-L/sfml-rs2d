#include "tile.h"

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

pair<bool, function<bool()>> Tile::click(Event event) {
	int x = event.mouseButton.x;
	int y = event.mouseButton.y;
	for (int i = 0; i < groundObjects.size(); ++i) {
		auto groundObject = groundObjects[i];
		auto gameObject = gameObjects[i];
		if (groundObject.isVisible && gameObject->hasRedClickAction && groundObject.isOverObject(x, y)) {
			auto interactions = gameObject->getInteractions();
			return make_pair(true, interactions[0].second);
		}
	}
	return make_pair(false, []() { return true; });
}
