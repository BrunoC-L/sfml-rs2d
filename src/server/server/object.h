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
	JSON json;
	const Tile* tile;
	unsigned id;
	std::string currentString;
	bool upToDate = false;
	int state;
public:
	Object(Tile* tile);
	unsigned getId();
	void setId(unsigned id);
	int getState();
	void setState(int state);
	const Tile* getTile();
	const JSON& asJSON();

	virtual const std::string& getName() = 0;
	virtual VTile size() = 0;
	virtual const std::vector<std::string>& getInteractions() = 0;
	virtual void interact(const std::shared_ptr<User>& user, int objectState, const std::string& interaction) = 0;
	virtual const std::vector<VTile>& getInteractibleTiles() = 0;
};
