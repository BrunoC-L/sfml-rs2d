#pragma once
#include "socketService.h"
#include "login.h"
#include <mutex>

class SocketMock2 : public Socket {
public:
	std::vector<std::string> msgs;
	std::mutex m;
	std::function<void(const std::string& type, const JSON& data)> _send;

	SocketMock2(ServiceProvider* provider) : Socket(provider, "0", 0) {
		provider->set(SOCKET, this);
	}

	virtual void send(const std::string& type, const JSON& data) override {
		_send(type, data);
	}

	virtual void connect(std::string ip, unsigned port) {}

	virtual void disconnect() {}

	virtual void mockReceiveFromServer(JSON json) {
		receive(json["type"].asString(), std::make_shared<const JSON>(json["data"]));
	}

	virtual void mockReceiveFromServer(std::string type, JSON json) {
		receive(type, std::make_shared<const JSON>(json));
	}
};
