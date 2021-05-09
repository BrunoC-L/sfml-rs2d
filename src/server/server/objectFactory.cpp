#include "objectFactory.h"
#include "Tree.h"

std::shared_ptr<Object> ObjectFactory::create(JSON& object) {
	return std::make_shared<Tree>();
}