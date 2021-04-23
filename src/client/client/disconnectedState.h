#pragma once
#include "socketState.h"

class DisconnectedSocketState : public SocketState {
public:
	DisconnectedSocketState(AbstractSocket* context) : SocketState(context) {}
	virtual void send(std::string) override;
	virtual std::string getName() override;
	virtual bool connected() override;
};
