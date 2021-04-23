#pragma once
#include "socketState.h"

class ConnectedSocketState : public SocketState {
public:
	ConnectedSocketState(AbstractSocket* context) : SocketState(context) {}
	virtual void send(std::string) override;
	virtual std::string getName() override;
	virtual bool connected() override;
};
