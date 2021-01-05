#pragma once
#include "tile.h"
#include "Tilemap.h"
#include "../../common/units.h"
#include "../../common/splitstring.h"
#include <unordered_map>
#include <fstream>
#include "GameObject.h"
#include "abstractMeasures.h"
#include "abstractRenderWindow.h"
#include "../../common/abstractServiceProvider.h"

class Chunk {
public:
	Chunk(const VChunk& pos, AbstractServiceProvider* provider);
	void draw(AbstractRenderWindow& w, const VTile& pos, const VChunk& chunkOffset);
	~Chunk();
	vector<vector<Tile*>> tiles;
private:
	AbstractServiceProvider* provider;
	sf::Transform getTransform(const VTile& relativePos, const VChunk& chunkOffset) const;
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
