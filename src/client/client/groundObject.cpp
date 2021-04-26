#include "groundObject.h"

GroundObject::GroundObject(int id, VTile position, int textureIndex, std::function<void(VTile, int)> updateObjectTexture, bool isVisible) :
	parentGameObjectID(id),
	position(position),
	textureIndex(textureIndex),
	updateObjectTexture(updateObjectTexture),
	isVisible(isVisible) {
	if (isVisible)
		show();
	isOverObject = [](int x,int y) {return true; };
}

void GroundObject::show() {
	isVisible = true;
	updateObjectTexture(position, textureIndex);
}

void GroundObject::hide() {
	if (!isVisible)
		return;
	isVisible = false;
	updateObjectTexture(position, 0);
}
