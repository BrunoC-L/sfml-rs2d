#pragma once
#include "socketService.h"
#include "login.h"
#include <mutex>

class SocketMock1 : public Socket {
public:
	SocketMock1(ServiceProvider* provider) : Socket(provider, "0", 0) {
		provider->set(SOCKET, this);
	}
	virtual void init() override {
		on("login",
			[&](std::shared_ptr<const JSON> data) {
				EVENT(LoginEvent, *data).emit();
			}
		);
	}
	virtual void connect(std::string ip, unsigned port) {}
	virtual void disconnect() {}
	virtual void mockReceiveFromServer(std::string type, const JSON& data) {
		receive(type, std::make_shared<const JSON>(data));
	}
};
