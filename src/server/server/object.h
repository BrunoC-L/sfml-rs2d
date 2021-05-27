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
	Tile* tile;
	unsigned id;
public:
	Object(JSON&& json, Tile* tile) : json(json), tile(tile), id(-1) {
		ObjectCreatedEvent(this).emit();
	}
	unsigned getId() { return id; }
	void setId(unsigned id) { this->id = id; }

	virtual VTile size() = 0;
	virtual Tile* getTile() { return tile; }
	virtual int getState() = 0;
	virtual const std::string& getName() = 0;
	virtual const std::vector<std::string>& getInteractions() = 0;
	virtual void interact(const std::shared_ptr<User>& user, int objectState, const std::string& interaction) = 0;
	virtual const std::vector<VTile>& getInteractibleTiles() = 0;
	virtual JSON asJSON();
};
