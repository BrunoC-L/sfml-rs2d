#include "connectedState.h"
#include "tcpSocket.h"

void ConnectedSocketState::send(const std::string& str) {
	context.sendNoCheck(str);
}

std::string ConnectedSocketState::getName() {
	return "Connected";
}

bool ConnectedSocketState::connected() {
	return true;
}
