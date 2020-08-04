#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <functional>

#include "NPC.h"
#include "groundItem.h"
#include "groundObject.h"
#include "units.h"

using namespace std;
using namespace sf;

class Tile {
public:
	Tile(int x, int y, int borders, const vector<GroundItem> groundItems, const vector<GroundObject> groundObjects, const vector<NPC> NPCs, const function<void(void)> callback);
	bool canMoveFrom(Tile from);
	const int borders;
private:
	const VTile position;
	const vector<GroundItem>& groundItems;
	const vector<GroundObject>& groundObjects;
	const vector<NPC>& NPCs;
	const function<void(void)>& callback;
};
