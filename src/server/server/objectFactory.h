#pragma once
#include "object.h"
#include <memory>
#include <string>
#include "json.h"

class Tile;

class ObjectFactory {
protected:
	virtual std::shared_ptr<Object> _create(std::string filename, JSON object, Tile* tile);
public:
	virtual std::shared_ptr<Object> create(std::string filename, JSON object, Tile* tile);
};
