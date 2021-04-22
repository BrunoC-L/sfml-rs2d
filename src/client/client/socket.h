#pragma once
#include "abstractSocket.h"
#include "service.h"
#include "jsonSocket.h"
#include <thread>
#include <vector>
#include <unordered_map>

class Socket : public AbstractSocket, public Service {
private:
	JSONTCPSocket socket;
public:
	std::thread listener;
	std::unordered_map<std::string, std::vector<std::function<void(JSON&)>>> callbacks;
	Socket(AbstractServiceProvider*);
	virtual void send(JSON&) override;
	virtual void on(std::string, std::function<void(JSON&)>) override;
	virtual void init() override;
	virtual void receive(std::string, JSON&) override;
	virtual void emit(std::string, JSON&) override;
	virtual void login() override;
	virtual void connect(std::string ip, unsigned port) override;
	virtual void disconnect() override;
};
