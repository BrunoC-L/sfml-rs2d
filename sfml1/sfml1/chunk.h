#pragma once
#include "tile.h"
#include "Tilemap.h"
#include "units.h"
#include "measures.h"
#include "Textures.h"

class Chunk {
public:
	Chunk(const VChunk& pos, Measures& measures, Textures& textures);
	void draw(RenderWindow& w, const VTile& pos, const VChunk& chunkOffset) const;
	~Chunk();
	vector<string> split(const string& s, const char c) const;
	vector<vector<Tile*>> tiles;
private:
	Transform getTransform(const VTile& relativePos, const VChunk& chunkOffset) const;
	Measures& measures;
	string getTilesetFileName() const;
	string getTexturesetFileName() const;
	string getWallsTexturesetFileName() const;
	VChunk chunkpos;
	TileMap tilemap;
	TileMap wallmap;
	int walls[int(Measures::TilesPerChunk * Measures::TilesPerChunk)];
	int level[int(Measures::TilesPerChunk * Measures::TilesPerChunk)];
};
