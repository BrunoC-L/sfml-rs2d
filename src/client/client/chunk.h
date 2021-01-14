#pragma once
#include "tile.h"
#include "Tilemap.h"
#include "../../common/common/units.h"
#include "../../common/common/splitstring.h"
#include <unordered_map>
#include <fstream>
#include "GameObject.h"
#include "abstractMeasures.h"
#include "abstractRenderWindow.h"
#include "../../common/common/abstractServiceProvider.h"

class Chunk {
public:
	Chunk(const VChunk& pos, AbstractServiceProvider* provider);
	~Chunk();
	vector<vector<Tile*>> tiles;
	bool deleted = false;
	TileMap tilemap;
	TileMap wallmap;
	TileMap objectmap;
private:
	AbstractServiceProvider* provider;
	string getTilesetFileName() const;
	string getGroundTexturesetFileName() const;
	string getObjectsTexturesetFileName() const;
	string getWallsTexturesetFileName() const;
	VChunk chunkpos;
	unordered_map<VTile, vector<GameObject*>, VTileHash> gameObjects;
};
