#pragma once
#include "abstractService.h"
#include "item.h"

class AbstractInventory : public AbstractService {
public:
	unsigned _space;
	unsigned size;
	std::vector<Item> items;
	virtual std::vector<Item> getItems() = 0;
	virtual bool has(std::vector<std::pair<Item, unsigned>> items) = 0;
	virtual bool add(std::vector<std::pair<Item, unsigned>> items) = 0;
	virtual bool remove(std::vector<std::pair<Item, unsigned>> items) = 0;
	virtual unsigned space() = 0;
	virtual bool has(Item item, unsigned quantity) = 0;
	virtual bool add(Item item, unsigned quantity) = 0;
	virtual bool remove(Item item) = 0;
};
