#pragma once
#include "tile.h"
#include "units.h"
#include "splitString.h"
#include <unordered_map>
#include <fstream>
#include <memory>
#include "objectFactory.h"
#include "constants.h"

class Chunk {
private:
	std::string getFileName(const std::string& type) const;
	std::string getObjectFileNameWithPath(const std::string& objectFileName) const;
	void loadWalls();
	void loadObjects();
	VChunk chunkpos;
	bool deleted = false;
	ObjectFactory& objectFactory;
public:
	Chunk(const VChunk& pos, ObjectFactory& objectFactory);
	~Chunk();
	Tile* tiles[int(TILES_PER_CHUNK)][int(TILES_PER_CHUNK)];
};
