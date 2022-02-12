#pragma once
#include "abstractSocketServer.h"
#include "service.h"
#include "JSONSFMLSocketServer.h"
#include "websocketppserver.h"

class SocketServerService : public AbstractSocketServer, public Service {
	std::unique_ptr<JSONSFMLSocketServer> socketServer;
	std::unique_ptr<JSONWebSocketServer> webSocketServer;
	std::unordered_map<std::shared_ptr<Socket>, std::shared_ptr<User>> socketToUser;
	std::unordered_map<std::shared_ptr<User>, std::shared_ptr<Socket>> userToSocket;
public:
	SocketServerService(ServiceProvider* provider, unsigned port);
	virtual void on(std::string msgType, std::function<void(std::shared_ptr<User>, const JSON&)> callback, bool loggedInRequired) override;
	//virtual void send(std::shared_ptr<User> user, const JSON& msg) override;
	virtual void send(std::shared_ptr<User> user, std::string type, const JSON& data) override;
	virtual void init() override;
	virtual void stop() override;
};
