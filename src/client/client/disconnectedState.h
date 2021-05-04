#pragma once
#include "socketState.h"

class DisconnectedSocketState : public SocketState {
public:
	DisconnectedSocketState(TCPSocket& context) : SocketState(context) {}
	virtual void send(const std::string&) override;
	virtual std::string getName() override;
	virtual bool connected() override;
};
