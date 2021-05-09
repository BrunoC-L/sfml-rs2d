#pragma once
#include "tile.h"
#include "units.h"
#include "splitString.h"
#include <unordered_map>
#include <fstream>
#include <memory>
#include "objectFactory.h"

class Chunk {
private:
	std::string getFileName(const std::string& type) const;
	void loadWalls();
	void loadObjects();
	VChunk chunkpos;
	bool deleted = false;
	ObjectFactory& objectFactory;
public:
	static constexpr float TilesPerChunk = 64;
	Chunk(const VChunk& pos, ObjectFactory& objectFactory);
	std::shared_ptr<Tile> tiles[int(TilesPerChunk)][int(TilesPerChunk)];
};
