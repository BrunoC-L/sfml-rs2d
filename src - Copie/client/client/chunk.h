#pragma once
#include "tile.h"
#include "Tilemap.h"
#include "units.h"
#include "splitstring.h"
#include <unordered_map>
#include <fstream>
#include "abstractServices.h"

class Chunk {
private:
	AbstractGameDataService* gameData;
	std::string getGroundTexturesetFileName() const;
	void loadTexture();
	VChunk chunkpos;
	void loadObjects();
	sf::Texture mapTexture;
	sf::Texture* objectsTexture;
	int* objects;
public:
	Chunk(const VChunk& pos, sf::Texture* objectsTexture, AbstractGameDataService* gameData);
	~Chunk();
	std::shared_ptr<Tile> tiles[TILES_PER_CHUNK][TILES_PER_CHUNK];
	sf::RectangleShape map;
	TileMap objectMap;
	int* getObjectsPtr();
	void updateInteractions(VTile tile, ObjectInteractions* interactions);
};
