#include "objectFactory.h"
#include "tile.h"
#include "char_array_crc32.h"
#include "string_crc32.h"
#include "resource.h"
#include "tree.h"
#include "rock.h"
#include "onExit.h"

#define entry(EnumName, fileNameInListFolder) EnumName = char_array_crc32(fileNameInListFolder),

// Object names are reused in various places so we have to prefix them with an underscore to avoid conflicts
enum OBJECTS {
	entry(_Tree, "Tree")
	entry(_Copper_Rock, "Copper Rock")
};

enum OBJECT_TYPES {
	entry(_Trees, "tree")
	entry(_Rocks, "rock")
};

std::shared_ptr<Object> ObjectFactory::create(std::string filename, JSON object, Tile* tile) {
	std::shared_ptr<Object> obj = _create(std::move(filename), std::move(object), tile);
	obj->build();
	return obj;
}

std::shared_ptr<Object> ObjectFactory::_create(std::string filename, JSON object, Tile* tile) {
	if (object.has("type")) {
		auto type = string_crc32(object.get("type").asString());
		switch (type) {
			case _Trees:
				return std::make_shared<Tree>(std::move(filename), std::move(object), tile);
			case _Rocks:
				return std::make_shared<Rock>(std::move(filename), std::move(object), tile);
		}
	}
	int exactType = string_crc32(filename);
	switch (exactType) {
		case _Tree:
		case _Copper_Rock:
		default:
			throw std::exception();
	}
}
