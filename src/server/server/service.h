#pragma once
#include "abstractServices.h"
#include "../../common/common/abstractServiceProvider.h"

class Service {
protected:
	AbstractServiceProvider* provider;
	AbstractMap* map;
	AbstractDB* dbService;
	AbstractUserService* userService;
	AbstractPlayerActionService* playerActionService;
	AbstractSocketServer* server;
public:
	Service(AbstractServiceProvider* provider) : provider(provider) { }

	void acquire() {
		map = (AbstractMap*)provider->get("Map");
		dbService = (AbstractDB*)provider->get("DB");
		userService = (AbstractUserService*)provider->get("User");
		playerActionService = (AbstractPlayerActionService*)provider->get("PlayerAction");
		server = (AbstractSocketServer*)provider->get("Server");
	}
};
