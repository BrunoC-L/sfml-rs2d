#include "objectFactory.h"
#include "Tree.h"
#include "tile.h"

std::shared_ptr<Object> ObjectFactory::create(JSON object, Tile* tile) {
	if (object.asString() != "")
		return std::make_shared<Tree>(std::move(object), tile);
	return std::shared_ptr<Object>(nullptr);
}
