#pragma once
#include "socketState.h"

class ConnectedSocketState : public SocketState {
public:
	ConnectedSocketState(TCPSocket& context) : SocketState(context) {}
	virtual void send(const std::string&) override;
	virtual std::string getName() override;
	virtual bool connected() override;
};
