#pragma once
#include "abstractSocket.h"
#include "service.h"
#include "jsonSocket.h"
#include <thread>
#include <vector>
#include <unordered_map>
#include "socketState.h"


class Socket : public AbstractSocket, public Service {
private:
	JSONTCPSocket socket;
	std::shared_ptr<SocketState> state;
	std::string ip;
	int port;
	std::thread* listener;
	std::unordered_map<std::string, std::vector<std::function<void(JSON&)>>> callbacks;
public:
	Socket(ServiceProvider*, std::string ip, int port);
	virtual void send(JSON&) override;
	virtual void on(std::string, std::function<void(JSON&)>) override;
	virtual void init() override;
	virtual void receive(std::string, JSON&) override;
	virtual void emit(std::string, JSON&) override;
	virtual bool connect() override;
	virtual void disconnect() override;
	virtual void send(std::string str) override;
	virtual void sendNoCheck(std::string str);
};
