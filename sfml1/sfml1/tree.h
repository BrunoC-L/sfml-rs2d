#pragma once
#include "gameRessourceObject.h"

class Tree : public GameRessourceObject {
public:
	Tree(int id, VTile position, function<void(VTile, int)> updateObjectTexture);
};
