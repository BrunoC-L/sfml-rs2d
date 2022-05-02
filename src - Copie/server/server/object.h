#pragma once
#include "units.h"
#include <memory>
#include <vector>
#include <string>
#include "json.h"
#include "objectCreatedEvent.h"

class User;
class Tile;

class Object {
protected:
	std::string fileName;
	JSON repr;
	JSON _textures;
	const Tile* tile;
	unsigned id;
	std::string currentString;
	bool upToDate = false;
	int state;
	virtual void updateRepresentation();
public:
	Object(std::string fileName, Tile* tile);
	/*unsigned getId();
	void setId(unsigned id);*/
	int getState();
	virtual void setState(int state);
	const Tile* getTile();
	const JSON& asJSON();
	const JSON& textures();

	virtual void build() = 0;

	virtual const std::string& getName() = 0;
	virtual VTile size() = 0;
	virtual const std::vector<std::string>& getInteractions() = 0;
	virtual void interact(const std::shared_ptr<User>& user, int objectState, const std::string& interaction) = 0;
	virtual const std::vector<VTile>& getInteractibleTiles() = 0;
};
