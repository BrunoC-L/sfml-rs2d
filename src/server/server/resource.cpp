#include "resource.h"
#include "tile.h"
#include "movingPredicate.h"

std::vector<VTile> Resource::getInteractibleTiles() {
	std::vector<VTile> occupied = {};
	const auto size = this->size();
	for (int x = 0; x < size.x; ++x)
		for (int y = 0; y < size.y; ++y)
			occupied.push_back(getTile()->position + VTile(x, y));
	return MovingPredicate::getNextTo(occupied);
}
