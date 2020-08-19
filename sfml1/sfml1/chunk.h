#pragma once
#include "tile.h"
#include "Tilemap.h"
#include "units.h"
#include "measures.h"
#include "splitString.h"
#include <unordered_map>
#include <fstream>
#include "GameObject.h"

class Chunk {
public:
	Chunk(const VChunk& pos);
	void draw(RenderWindow& w, const VTile& pos, const VChunk& chunkOffset);
	~Chunk();
	vector<vector<Tile*>> tiles;
private:
	Transform getTransform(const VTile& relativePos, const VChunk& chunkOffset) const;
	string getTilesetFileName() const;
	string getGroundTexturesetFileName() const;
	string getObjectsTexturesetFileName() const;
	string getWallsTexturesetFileName() const;
	VChunk chunkpos;
	TileMap tilemap;
	TileMap wallmap;
	TileMap objectmap;
	unordered_map<VTile, vector<GameObject*>, VTileHash> gameObjects;
	bool deleted = false;
};
