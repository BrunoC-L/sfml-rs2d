#pragma once
#include "abstractServices.h"
#include "../../common/abstractServiceProvider.h"

class Service {
protected:
	AbstractServiceProvider* provider;
	AbstractMap* map;
public:
	Service(AbstractServiceProvider* provider) : provider(provider) { }

	void acquire() {
		map = (AbstractMap*)provider->get("Map");
	}
};
