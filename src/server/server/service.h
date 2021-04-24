#pragma once
#include "abstractServices.h"
#include "ServiceProvider.h"

class Service {
protected:
	ServiceProvider* provider;
	AbstractMap* map;
	AbstractDB* dbService;
	AbstractUserService* userService;
	AbstractPlayerActionService* playerActionService;
	AbstractSocketServer* server;
public:
	Service(ServiceProvider* provider) : provider(provider) { }

	void acquire() {
		map = (AbstractMap*)provider->get("Map");
		dbService = (AbstractDB*)provider->get("DB");
		userService = (AbstractUserService*)provider->get("User");
		playerActionService = (AbstractPlayerActionService*)provider->get("PlayerAction");
		server = (AbstractSocketServer*)provider->get("Server");
	}
};
