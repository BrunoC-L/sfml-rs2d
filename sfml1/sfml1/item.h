#pragma once
#include "splitString.h"
using namespace std;

class Item {
public:
	static Item fromString(string s, string divider);
	unsigned id;
	string name;
	unsigned textureIndex;
	bool stackable;
	unsigned weight;
	unsigned quantity;
	Item(unsigned id, string name, unsigned textureIndex, bool stackable, unsigned weight, unsigned quantity) :
		id(id), 
		name(name), 
		textureIndex(textureIndex),
		stackable(stackable),
		weight(weight),
		quantity(quantity)
	{ }
	Item();
	bool operator==(const Item& other) { return id == other.id; }
};
