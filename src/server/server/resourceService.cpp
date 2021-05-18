#include "resourceService.h"

ResourceService::ResourceService(ServiceProvider* provider) : Service(provider) {
	provider->set(RESOURCE, this);
}

void ResourceService::init() {
	acquire();
}

void ResourceService::interact(const std::shared_ptr<User>& user, VTile v, int objectState, int interactionIndex, int objectId) {
	Tile* tile = map->getTile(v);
	if (!tile)
		return;
	const std::shared_ptr<Object>* object = tile->getObject(objectId);
	if (!object || !*object)
		return;
	(*object)->interact(user, objectState, interactionIndex);
}
