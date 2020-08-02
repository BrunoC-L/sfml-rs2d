#pragma once
#include "tile.h"
#include "Tilemap.h"
#include "units.h"
#include "measures.h"

class Chunk {
public:
	Chunk(const VChunk& pos, Measures& measures);
	void draw(RenderWindow& w, const VTile& pos, const VChunk& chunkOffset) const;
	~Chunk();
	vector<string> split(const string& s, const char c) const;
private:
	Measures& measures;
	string getTilesetFileName() const;
	string getTexturesetFileName() const;
	VChunk chunkpos;
	vector<vector<Tile*>> tiles;
	TileMap tilemap;
	int level[64 * 64];
};