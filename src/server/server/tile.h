#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <functional>
#include "units.h"
#include "object.h"

class Tile {
protected:
	std::vector<std::shared_ptr<Object>> objects;
public:
	Tile(int x, int y, int borders);
	bool canMoveFrom(Tile from);
	bool canMove(VTile delta, int bordersTo, int bordersFrom);
	const int borders;
	const VTile position;
	bool walkable;
	void addObject(std::shared_ptr<Object>&& object);
};
