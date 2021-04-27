#pragma once
#include <functional>
#include "units.h"

class GroundObject {
public:
	GroundObject(int id, VTile position, int textureIndex, std::function<void(VTile, int)> updateObjectTexture, bool isVisible);
	void show();
	void hide();
	int parentGameObjectID;
	VTile position;
	int textureIndex;
	std::function<bool(int, int)> isOverObject;
	bool isVisible;
	std::function<void(VTile, int)> updateObjectTexture;
};
