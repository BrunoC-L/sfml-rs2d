#pragma once
#include <SFML/Graphics.hpp>
#include "NPC.h"
#include "callback.h"
#include "groundItem.h"
#include "groundObject.h"
using namespace std;
using namespace sf;

class Tile {
public:
	Tile(int borders, const vector<GroundItem> groundItems, const vector<GroundObject> groundObjects, const vector<NPC> NPCs, const vector<Callback> callbacks);
};
