#pragma once
#include "item.h"
#include <vector>
#include <algorithm>
#include "abstractServices.h"
#include "service.h"

class Inventory : public AbstractInventory, public Service {
public:
	Inventory(ServiceProvider* provider) : Service(provider) {
		provider->set("Inventory", this);
		size = 28;
		_space = size;
		items = std::vector<Item>(28, Item());
	}
	void init() {
		acquire();
	}
	virtual bool has(std::vector<std::pair<Item, unsigned>> items);
	virtual bool add(std::vector<std::pair<Item, unsigned>> items);
	virtual bool remove(std::vector<std::pair<Item, unsigned>> items);
	virtual unsigned space();
	virtual bool has(Item item, unsigned quantity);
	virtual bool add(Item item, unsigned quantity);
	virtual bool remove(Item item);
	virtual std::vector<Item> getItems();
};
