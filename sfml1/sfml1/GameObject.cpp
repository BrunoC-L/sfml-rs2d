#include "GameObject.h"

GameObject::GameObject(int id, function<void(VTile, int)> updateObjectTexture) : id(id), updateObjectTexture(updateObjectTexture) {
	hasRedClickAction = true;
}

vector<pair<string, function<bool()>>> GameObject::getInteractions() {
	return vector<pair<string, function<bool()>>>();
}

string GameObject::getName() {
	return name;
}
