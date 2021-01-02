#include "item.h"
#include <string>

Item::Item() : id(0), name(""), textureIndex(0), stackable(true), weight(0), quantity(0) { }

Item Item::fromString(string s, string divider) {
	auto arr = split(s, divider);
	return Item(stoi(arr[0]), arr[1], stoi(arr[2]), arr[3] == "true", stoi(arr[4]), stoi(arr[5]));
}
