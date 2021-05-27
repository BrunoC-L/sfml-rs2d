#pragma once
#include "socketService.h"
#include <mutex>

class SocketMock1 : public Socket {
public:
	std::vector<std::string> msgs;
	std::mutex m;
	SocketMock1(ServiceProvider* provider);
	virtual void init();
	virtual void connect(std::string ip, unsigned port);
	virtual void disconnect();
	virtual void mockReceiveFromServer(JSON json);
};
