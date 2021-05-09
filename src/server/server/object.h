#pragma once
#include "units.h"
#include <memory>
#include <vector>
#include <string>

class User;
class Tile;

class Object {
public:
	virtual VTile size() = 0;
	virtual std::vector<std::string> getInteractions() = 0;
	virtual void interact(const std::shared_ptr<User>& user, int objectState, int index) = 0;
	virtual int getState() = 0;
	virtual std::string getName() = 0;
};
