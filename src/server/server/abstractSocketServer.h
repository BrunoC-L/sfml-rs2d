#pragma once
#include "../../common/common/abstractService.h"
#include <functional>
#include "../../common/common/json.h"
#include <SFML/Network.hpp>

class User;

class AbstractSocketServer : public AbstractService {
public:
	virtual void on(std::string msgType, std::function<void(std::shared_ptr<User>, JSON)> callback, bool loggedInRequired) = 0;
	virtual void send(std::shared_ptr<User> user, JSON msg) = 0;
	virtual void send(std::shared_ptr<User> user, std::string type, JSON data) = 0;
};
