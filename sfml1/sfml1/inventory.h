#pragma once
#include "item.h"
#include <vector>
#include <algorithm>

class Inventory {
private:
	unsigned _space;
	unsigned size;
	vector<Item> items;
public:
	Inventory(unsigned size, vector<Item> items);
	bool has(vector<pair<Item, unsigned>> items);
	bool add(vector<pair<Item, unsigned>> items);
	bool remove(vector<pair<Item, unsigned>> items);
	unsigned space();
	bool has(Item item, unsigned quantity);
	bool add(Item item, unsigned quantity);
	bool remove(Item item);
	vector<Item> getItems();
};
