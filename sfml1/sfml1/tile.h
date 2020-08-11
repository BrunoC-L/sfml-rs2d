#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <functional>

#include "NPC.h"
#include "groundItem.h"
#include "GameObject.h"
#include "units.h"

using namespace std;
using namespace sf;

class Tile {
public:
	Tile(int x, int y, int borders, const vector<GroundItem> groundItems, const vector<GameObject*> groundObjects, const vector<NPC> NPCs, const function<void()> callback);
	bool canMoveFrom(Tile from);
	const int borders;
	const VTile position;
	pair<bool, function<bool()>> click(Event event);
private:
	vector<GameObject*> gameObjects;
	vector<GroundObject> groundObjects;
	vector<GroundItem> groundItems;
	vector<NPC> NPCs;
	function<void()> callback;
};
