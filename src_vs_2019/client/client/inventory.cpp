#include "inventory.h"

bool Inventory::has(Item item, unsigned quantity) {
	for (auto i = 0; i < size && quantity; ++i)
		if (items[i].isSameType(item))
			if (item.stackable)
				return items[i].quantity >= quantity;
			else
				quantity -= 1;
	return !quantity;
}

bool Inventory::has(std::vector<std::pair<Item, unsigned>> items) {
	for (auto i = 0; i < items.size(); ++i)
		for (auto j = i + 1; j < items.size(); ++j)
			if (items[i].first.isSameType(items[j].first))
				throw std::exception("vector for inventory::has has duplicates");
	for (auto p : items)
		if (!has(p.first,  p.second))
			return false;
	return true;
}

bool Inventory::add(std::vector<std::pair<Item, unsigned>> items) {
	unsigned count = 0;
	for (auto p : items)
		count += p.second;
	if (count > _space)
		return false;
	for (auto p : items)
		for (auto i = 0; i < p.second; ++i)
			if (!add(p.first, p.second))
				throw std::exception("Failed to add item");
	return true;
}

bool Inventory::remove(std::vector<std::pair<Item, unsigned>> items) {
	for (auto p : items)
		if (!has(p.first, p.second))
			return false;
	for (auto p : items)
		for (auto i = 0; i < p.second; ++i)
			if (!remove(p.first))
				throw std::exception("Failed to remove item");
	return true;
}

unsigned Inventory::space() {
	return _space;
}

bool Inventory::add(Item item, unsigned quantity) {
	_ASSERT(item.id);
	if (item.stackable && has(item, 1))
		for (auto i = 0; i < size; ++i)
			if (items[i].id == item.id) {
				items[i]. quantity += quantity;
				return true;
			}
	if (_space == 0)
		return false;
	for (auto i = 0; i < size; ++i)
		if (!items[i].id) {
			items[i] = item;
			_space -= 1;
			return true;
		}
	throw std::exception("space check passed but couldnt find empty space for item to be added");
}

bool Inventory::remove(Item item) {
	_ASSERT(item.id);
	for (auto i = 0; i < size; ++i)
		if (items[i].isSameType(item)) {
			if (item.stackable) {
				_ASSERT(items[i].quantity >= item.quantity);
				items[i].quantity -= item.quantity;
			}
			if (!item.stackable || items[i].quantity == 0) {
				items[i] = Item();
				_space += 1;
			}
			return true;
		}
	return false;
}

std::vector<Item> Inventory::getItems() {
	return items;
}
