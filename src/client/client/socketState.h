#pragma once
#include <string>
#include "abstractSocket.h"

class SocketState {
protected:
	AbstractSocket* context;
public:
	SocketState(AbstractSocket* context) : context(context) {}
	virtual void send(std::string) = 0;
	virtual std::string getName() = 0;
	virtual bool connected() = 0;
};
