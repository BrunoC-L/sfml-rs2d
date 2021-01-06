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
	std::thread listener;
	std::unordered_map<std::string, std::vector<std::function<void(JSON)>>> callbacks;
public:
	Socket(AbstractServiceProvider*);
	virtual void send(JSON);
	virtual void on(std::string, std::function<void(JSON)>);
	virtual void init();
	virtual void receive(std::string, JSON);
	virtual void emit(std::string, JSON);
	virtual void login();
};
