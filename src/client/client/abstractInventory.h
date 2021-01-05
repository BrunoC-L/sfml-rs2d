#pragma once
#include "../../common/abstractService.h"
#include "item.h"

class AbstractInventory : public AbstractService {
public:
	unsigned _space;
	unsigned size;
	vector<Item> items;
	virtual vector<Item> getItems() = 0;
	virtual bool has(vector<pair<Item, unsigned>> items) = 0;
	virtual bool add(vector<pair<Item, unsigned>> items) = 0;
	virtual bool remove(vector<pair<Item, unsigned>> items) = 0;
	virtual unsigned space() = 0;
	virtual bool has(Item item, unsigned quantity) = 0;
	virtual bool add(Item item, unsigned quantity) = 0;
	virtual bool remove(Item item) = 0;
};
