#pragma once
#include "tile.h"
#include "Tilemap.h"
#include "units.h"
#include "measures.h"
#include "Textures.h"
#include "splitString.h"

class Chunk {
public:
	Chunk(const VChunk& pos, Measures& measures, Textures& textures);
	void draw(RenderWindow& w, const VTile& pos, const VChunk& chunkOffset);
	~Chunk();
	vector<vector<Tile*>> tiles;
private:
	Transform getTransform(const VTile& relativePos, const VChunk& chunkOffset) const;
	Measures& measures;
	string getTilesetFileName() const;
	string getGroundTexturesetFileName() const;
	string getObjectsTexturesetFileName() const;
	string getWallsTexturesetFileName() const;
	VChunk chunkpos;
	TileMap tilemap;
	TileMap wallmap;
	TileMap objectmap;
};
