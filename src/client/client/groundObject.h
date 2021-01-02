#pragma once
#include <SFML/Graphics.hpp>
#include "abstractMeasures.h"

using namespace std;

class GroundObject {
public:
	GroundObject(int id, VTile position, int textureIndex, function<void(VTile, int)> updateObjectTexture, bool isVisible);
	void show();
	void hide();
	int parentGameObjectID;
	VTile position;
	int textureIndex;
	function<bool(int, int)> isOverObject;
	bool isVisible;
	function<void(VTile, int)> updateObjectTexture;
};
