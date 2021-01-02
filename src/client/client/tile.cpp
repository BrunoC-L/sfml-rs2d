#include "tile.h"
#include "pathfinder.h"

Tile::Tile(int x, int y, int borders, vector<GroundItem> groundItems, vector<GameObject*> gameObjects, vector<NPC> NPCs, function<void()> callback)
	: position(x, y), borders(borders), groundItems(groundItems), NPCs(NPCs), callback(callback) {
	if (gameObjects.size())
		for (auto gameObject : gameObjects)
			for (auto groundObject : gameObject->groundObjects)
				if (position == groundObject.position) {
					this->gameObjects.push_back(gameObject);
					groundObjects.push_back(groundObject);
				}
	walkable = borders != 0b1111;
}

bool Tile::canMoveFrom(Tile from) {
	VTile delta = from.position - position;
	return canMove(delta, borders, from.borders);
}

bool Tile::canMove(VTile delta, int bordersTo, int bordersFrom) {
	constexpr int east = 10,     north = -1,     west = -10,    south = 1;
	constexpr int EAST = 0b1000, NORTH = 0b0100, WEST = 0b0010, SOUTH = 0b0001;
	const int encoding = 10 * delta.x + delta.y;
	if (encoding > 0)
		return canMove(VTile() - delta, bordersFrom, bordersTo);
	switch (encoding) {
		case   0: return true;
		case  west + north : return !(bordersTo & (NORTH | WEST) || bordersFrom & (SOUTH | EAST));
		case  west + south : return !(bordersTo & (SOUTH | WEST) || bordersFrom & (NORTH | EAST));
		case  west         : return !(bordersTo &  WEST          || bordersFrom &  EAST);
		case  north        : return !(bordersTo &  NORTH         || bordersFrom &  SOUTH);
		default:
			throw new exception("This should never happen");
	}
}

function<bool()> Tile::onLeftClick(MouseLeftClickEvent event) {
	bool redClick = false;
	for (int i = 0; i < groundObjects.size(); ++i) {
		auto groundObject = groundObjects[i];
		auto gameObject = gameObjects[i];
		string name = gameObject->getName();
		if (groundObject.isVisible && groundObject.isOverObject(1, 1)) {
			auto interactions = gameObject->getInteractions();
			if (interactions.size() && interactions[0].first != "Examine") {
				InteractionClickEvent(interactions[0].second).emit();
				redClick = true;
				break;
			}
		}
	}
	if (!redClick)
		WalkClickEvent(position).emit();
	return []() { return false; };
}

function<bool()> Tile::onRightClick(MouseRightClickEvent event) {
	RightClickTileEvent(this).emit();
	return []() { return false; };
}

function<bool()> Tile::onMiddleClick(MouseMiddleClickEvent event) {
	TeleportEvent(position).emit();
	return []() { return false; };
}

function<bool()> Tile::onMove(MouseMoveEvent event) {
	return []() { return false; };
}
