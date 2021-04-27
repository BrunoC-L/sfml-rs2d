#pragma once
#include "tile.h"
#include "Tilemap.h"
#include "units.h"
#include "splitstring.h"
#include <unordered_map>
#include <fstream>
#include "GameObject.h"
#include "abstractMeasures.h"

class Chunk {
public:
	Chunk(const VChunk& pos);
	~Chunk();
	std::shared_ptr<Tile> tiles[int(AbstractMeasures::TilesPerChunk)][int(AbstractMeasures::TilesPerChunk)];
	bool deleted = false;
	sf::Texture mapTexture;
	sf::RectangleShape map;
	TileMap wallmap;
	//int walls[int(AbstractMeasures::TilesPerChunk * AbstractMeasures::TilesPerChunk)];
private:
	std::string getGroundTexturesetFileName() const;
	//std::string getWallsTexturesetFileName() const;
	//std::string getWallsFileName() const;
	void loadTexture();
	//void loadWalls();
	VChunk chunkpos;
};
