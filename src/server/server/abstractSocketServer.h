#pragma once
#include "abstractService.h"
#include <functional>
#include "json.h"
#include <SFML/Network.hpp>
#include "packets.h"

class User;

class AbstractSocketServer : public AbstractService {
public:
	virtual void on(std::string msgType, std::function<void(std::shared_ptr<User>, JSON&)> callback, bool loggedInRequired) = 0;
	virtual void send(std::shared_ptr<User> user, JSON& msg) = 0;
	virtual void send(std::shared_ptr<User> user, std::string type, JSON& data) = 0;
	virtual void stop() = 0;
};
