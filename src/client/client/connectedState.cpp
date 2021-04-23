#include "connectedState.h"

void ConnectedSocketState::send(std::string str) {
	context->sendNoCheck(str);
}

std::string ConnectedSocketState::getName() {
	return "Connected";
}

bool ConnectedSocketState::connected() {
	return true;
}
