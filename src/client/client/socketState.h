#pragma once
#include <string>

class TCPSocket;

class SocketState {
protected:
	TCPSocket& context;
public:
	SocketState(TCPSocket& context) : context(context) {}
	virtual void send(const std::string&) = 0;
	virtual std::string getName() = 0;
	virtual bool connected() = 0;
};
