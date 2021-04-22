#pragma once
#include "abstractSocketServer.h"
#include "service.h"
#include "json-socket-server.h"

class SocketServerService : public AbstractSocketServer, public Service {
	JsonSocketServer* server;
	std::unordered_map<sf::TcpSocket*, std::shared_ptr<User>> socketToUser;
	std::unordered_map<std::shared_ptr<User>, sf::TcpSocket*> userToSocket;
public:
	SocketServerService(AbstractServiceProvider* provider, unsigned port);
	virtual void on(std::string msgType, std::function<void(std::shared_ptr<User>, JSON&)> callback, bool loggedInRequired);
	virtual void send(std::shared_ptr<User> user, JSON& msg);
	virtual void send(std::shared_ptr<User> user, std::string type, JSON& data);
	virtual void init();
};
