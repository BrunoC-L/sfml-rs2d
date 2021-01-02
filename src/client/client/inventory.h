#pragma once
#include "item.h"
#include <vector>
#include <algorithm>
#include "abstractServices.h"

class Inventory : public AbstractInventory {
public:
	Inventory(AbstractServiceProvider* provider) {
		REGISTER(Inventory);
		size = 28;
		_space = size;
		items = vector<Item>(28, Item());
	}
	void init() {
		ACQUIRE
	}
	virtual bool has(vector<pair<Item, unsigned>> items);
	virtual bool add(vector<pair<Item, unsigned>> items);
	virtual bool remove(vector<pair<Item, unsigned>> items);
	virtual unsigned space();
	virtual bool has(Item item, unsigned quantity);
	virtual bool add(Item item, unsigned quantity);
	virtual bool remove(Item item);
	virtual vector<Item> getItems();
};
