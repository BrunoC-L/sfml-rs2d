#include "inventory.h"

Inventory::Inventory(unsigned size, vector<Item> items) : size(size), items(items) {
	unsigned sum = 0;
	for (auto i : items)
		sum += i.id > 0;
	_space = size - sum;
	for (auto i = 0; i < _space; ++i)
		this->items.push_back(Item());
	_ASSERT(_space >= 0);
}

bool Inventory::has(Item item, unsigned quantity) {
	for (auto i = 0; i < size && quantity; ++i)
		if (items[i] == item)
			quantity -= 1;
	return !quantity;
}

bool Inventory::has(vector<pair<Item, unsigned>> items) {
	for (auto p : items)
		if (!has(p.first,  p.second))
			return false;
	return true;
}

bool Inventory::add(vector<pair<Item, unsigned>> items) {
	unsigned count = 0;
	for (auto p : items)
		count += p.second;
	if (count > _space)
		return false;
	for (auto p : items)
		for (auto i = 0; i < p.second; ++i)
			if (!add(p.first))
				throw new exception("Failed to add item");
	return true;
}

bool Inventory::remove(vector<pair<Item, unsigned>> items) {
	for (auto p : items)
		if (!has(p.first, p.second))
			return false;
	for (auto p : items)
		for (auto i = 0; i < p.second; ++i)
			if (!remove(p.first))
				throw new exception("Failed to remove item");
	return true;
}

unsigned Inventory::space() {
	return _space;
}

bool Inventory::add(Item item) {
	_ASSERT(item.id);
	if (_space == 0)
		return false;
	for (auto i = 0; i < size; ++i)
		if (!items[i].id) {
			items[i] = item;
			_space -= 1;
			return true;
		}
	throw new exception("space check passed but couldnt find empty space for item to be added");
}

bool Inventory::remove(Item item) {
	_ASSERT(item.id);
	for (auto i = 0; i < size; ++i)
		if (items[i] == item) {
			items.erase(items.begin() + i);
			items.insert(items.begin() + i, Item());
			_space += 1;
			return true;
		}
	return false;
}
