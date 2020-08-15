#include "item.h"
#include <string>

Item::Item() : id(0), name(""), textureIndex(0), stackable(true), weight(0) { }

Item Item::fromString(string s) {
	auto arr = split(s, ';');
	return Item(stoi(arr[0]), arr[1], stoi(arr[2]), arr[3] == "true", stoi(arr[4]));
}
