#pragma once
#include "splitString.h"
using namespace std;

class Item {
public:
	static Item fromString(string s);
	unsigned id;
	string name;
	unsigned textureIndex;
	bool stackable;
	unsigned weight;
	Item(unsigned id, string name, unsigned textureIndex, bool stackable, unsigned weight) :
		id(id), 
		name(name), 
		textureIndex(textureIndex),
		stackable(stackable),
		weight(weight)
	{ }
	Item();
	bool operator==(const Item& other) { return id == other.id; }
};
