#pragma once
#include "socketService.h"
#include "login.h"
#include <mutex>

class SocketMock1 : public Socket {
public:
	std::vector<std::string> msgs;
	std::mutex m;
	SocketMock1(ServiceProvider* provider) : Socket(provider, "0", 0) {
		provider->set(SOCKET, this);
	}
	virtual void init() {
		on("login",
			[&](std::shared_ptr<const JSON> data) {
				LoginEvent(*data).emit();
			}
		);
	}
	virtual void connect(std::string ip, unsigned port) {}
	virtual void disconnect() {}
	virtual void mockReceiveFromServer(JSON json) {
		receive(json["type"].asString(), std::make_shared<const JSON>(json["data"]));
	}
};
