#pragma once
#include <unordered_map>
#include <unordered_set>
#include "movingPredicate.h"

enum class PathFindOption { Onto, NextTo, NextToOrOnto };

class Map;

class Pathfinder {
public:
	static std::vector<VTile> pathfind(VTile a, std::vector<VTile> b, PathFindOption opt, Map* map);
protected:
	static std::vector<VTile> pathfind(VTile a, std::vector<VTile> b, Map* map);
};
