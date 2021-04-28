#pragma once
#include "abstractSocketServer.h"
#include "service.h"
#include "json-socket-server.h"

class SocketServerService : public AbstractSocketServer, public Service {
	JsonSocketServer* server;
	std::unordered_map<sf::TcpSocket*, std::shared_ptr<User>> socketToUser;
	std::unordered_map<std::shared_ptr<User>, sf::TcpSocket*> userToSocket;
public:
	SocketServerService(ServiceProvider* provider, unsigned port);
	virtual void on(std::string msgType, std::function<void(std::shared_ptr<User>, JSON&)> callback, bool loggedInRequired) override;
	virtual void send(std::shared_ptr<User> user, JSON& msg) override;
	virtual void send(std::shared_ptr<User> user, std::string type, JSON& data) override;
	virtual void init() override;
	virtual void stop() override;
};
