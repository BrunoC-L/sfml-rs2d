#pragma once
#include "../../common/abstractService.h"
#include <functional>
#include "../../common/json.h"
#include <SFML/Network.hpp>

class User;

class AbstractSocketServer : public AbstractService {
public:
	std::vector< std::shared_ptr<User>> users;
	virtual void on(std::string msgType, std::function<void(std::shared_ptr<User>, JSON)> callback) = 0;
	virtual void send(std::shared_ptr<User> user, JSON msg) = 0;
	virtual void send(std::shared_ptr<User> user, std::string type, JSON data) = 0;
};
