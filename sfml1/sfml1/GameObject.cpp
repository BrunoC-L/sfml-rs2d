#include "GameObject.h"

GameObject::GameObject(int id, int* const tilemapPtr, function<void(VTile, int)> updateObjectTexture) : id(id), tilemapPtr(tilemapPtr), updateObjectTexture(updateObjectTexture) {
	hasRedClickAction = true;
}

vector<pair<string, function<bool()>>> GameObject::getInteractions() {
	return vector<pair<string, function<bool()>>>();
}
