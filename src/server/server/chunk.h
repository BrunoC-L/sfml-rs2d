#pragma once
#include "tile.h"
#include "../../common/units.h"
#include "../../common/splitString.h"
#include <unordered_map>
#include <fstream>
// #include "GameObject.h"
#include "../../common/abstractServiceProvider.h"

class Chunk {
public:
	Chunk(const VChunk& pos, AbstractServiceProvider* provider);
	~Chunk();
	vector<vector<Tile*>> tiles;
	static constexpr float TilesPerChunk = 64;
private:
	AbstractServiceProvider* provider;
	string getTilesetFileName() const;
	VChunk chunkpos;
	// unordered_map<VTile, vector<GameObject*>, VTileHash> gameObjects;
	bool deleted = false;
};
