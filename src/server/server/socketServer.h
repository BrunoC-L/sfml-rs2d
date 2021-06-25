#pragma once
#include <functional>
#include "json.h"
#include <SFML/Network.hpp>
#include "packets.h"
#include "service.h"
#include "json-socket-server.h"
#include "user.h"

class SocketServerService : public Service {
	std::unique_ptr<JsonSocketServer> server;
	std::unordered_map<std::shared_ptr<sf::TcpSocket>, std::shared_ptr<User>> socketToUser;
	std::unordered_map<std::shared_ptr<User>, std::shared_ptr<sf::TcpSocket>> userToSocket;
public:
	SocketServerService(ServiceProvider* provider, unsigned port);
	virtual void on(std::string msgType, std::function<void(std::shared_ptr<User>, JSON&)> callback, bool loggedInRequired);
	virtual void send(std::shared_ptr<User> user, JSON& msg);
	virtual void send(std::shared_ptr<User> user, std::string type, JSON& data);
	virtual void init();
	virtual void stop();
};
