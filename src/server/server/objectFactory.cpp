#include "objectFactory.h"
#include "tile.h"
#include "char_array_crc32.h"
#include "string_crc32.h"
#include "resource.h"

#define entry(EnumName, fileNameInListFolder) EnumName = char_array_crc32(fileNameInListFolder),

// Object names are reused in various places so we have to prefix them with an underscore to avoid conflicts
enum OBJECTS {
	entry(_Tree, "Tree")
	entry(_Copper_Rock, "Copper Rock")
};

std::shared_ptr<Object> ObjectFactory::create(std::string filename, JSON object, Tile* tile) {
	int crc = string_crc32(filename);
	switch (crc) {
		case _Tree:
		case _Copper_Rock:
			return std::make_shared<Resource>(std::move(object), tile);
		default:
			throw std::exception();
	}
}
