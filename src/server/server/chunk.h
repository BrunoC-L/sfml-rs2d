#pragma once
#include "tile.h"
#include "units.h"
#include "splitString.h"
#include <unordered_map>
#include <fstream>
#include <memory>

class Chunk {
private:
	std::string getWallsFileName() const;
	void loadWalls();
	VChunk chunkpos;
	bool deleted = false;
public:
	static constexpr float TilesPerChunk = 64;
	Chunk(const VChunk& pos);
	std::shared_ptr<Tile> tiles[int(TilesPerChunk)][int(TilesPerChunk)];
};
