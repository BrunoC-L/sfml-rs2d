#pragma once
#include "abstractSocket.h"
#include "service.h"
#include "tcpSocket.h"
#include <thread>
#include <vector>
#include <unordered_map>
#include "socketState.h"

class Socket : public AbstractSocket, public Service {
private:
	TCPSocket socket;
	std::shared_ptr<SocketState> state;
	std::string ip;
	int port;
	std::shared_ptr<std::thread> listener;
	std::unordered_map<std::string, std::vector<std::function<void(std::shared_ptr<const JSON>)>>> callbacks;
public:
	Socket(ServiceProvider*, std::string ip, int port);
	virtual void init() override;
	virtual void send(const JSON& message) override;
	virtual void send(const std::string& message) override;
	virtual void sendNoCheck(const std::string& message) override;
	virtual void on(const std::string& type, std::function<void(const std::shared_ptr<const JSON>&)>) override;
	virtual void emit(const std::string& type, const JSON& data) override;
	virtual void receive(const std::string& type, std::shared_ptr<const JSON> data) override;
	virtual bool connect() override;
	virtual void disconnect() override;
};
