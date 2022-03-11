#include "tile.h"
#include "clamp.h"

Tile::Tile(int x, int y, int borders) : position(x, y), borders(borders) {
	//if (gameObjects.size())
	//	for (auto gameObject : gameObjects)
	//		for (auto groundObject : gameObject->groundObjects)
	//			if (position == groundObject.position) {
	//				this->gameObjects.push_back(gameObject);
	//				groundObjects.push_back(groundObject);
	//			}
	walkable = borders != 0b1111;
}

bool Tile::canMoveFrom(Tile from) {
	VTile delta = from.position - position;
	return canMove(delta, borders, from.borders);
}

bool Tile::canMove(VTile delta, int bordersTo, int bordersFrom) {
	constexpr int                north = -1,     west = -10,    south = 1;
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
			throw std::runtime_error("This should never happen");
	}
}

void Tile::addObject(std::shared_ptr<Object> object) {
	objects.emplace_back(object);
	//objects.back()->setId(objects.size() - 1);
}

const std::shared_ptr<Object>* Tile::getObject(std::string name) {
	for (int index = 0; index < objects.size(); ++index)
		if (objects[index]->getName() == name)
			return &objects[index];
	return nullptr;
}
