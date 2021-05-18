#pragma once
#include "tile.h"
#include "units.h"
#include "splitString.h"
#include <unordered_map>
#include <fstream>
#include <memory>
#include "objectFactory.h"

 constexpr float TilesPerChunk = 64;

class Chunk {
private:
	std::string getFileName(const std::string& type) const;
	void loadWalls();
	void loadObjects();
	VChunk chunkpos;
	bool deleted = false;
	ObjectFactory& objectFactory;
public:
	Chunk(const VChunk& pos, ObjectFactory& objectFactory);
	~Chunk();
	Tile* tiles[int(TilesPerChunk)][int(TilesPerChunk)];
};
