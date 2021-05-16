#include "resourceService.h"

void ResourceService::init() {
	provider->set(RESOURCE, this);
}

void ResourceService::stop() {
}
