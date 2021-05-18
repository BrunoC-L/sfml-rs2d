#pragma once
#include "units.h"
#include <memory>
#include <vector>
#include <string>
#include "json.h"

class User;
class Tile;

class Object {
	JSON json;
	Tile* tile;
	unsigned id;
public:
	Object(JSON&& json, Tile* tile) : json(json), tile(tile), id(-1) {}
	~Object() {
		throw 1;
	}
	unsigned getId() { return id; }
	void setId(unsigned id) { this->id = id; }

	virtual VTile size() = 0;
	virtual Tile* getTile() { return tile; }
	virtual int getState() = 0;
	virtual std::string getName() = 0;
	virtual std::vector<std::string> getInteractions() = 0;
	virtual void interact(const std::shared_ptr<User>& user, int objectState, int index) = 0;
	virtual std::vector<VTile> getInteractibleTiles() = 0;
};
