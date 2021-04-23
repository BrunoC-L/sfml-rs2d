#include "disconnectedState.h"

void DisconnectedSocketState::send(std::string str) {
	// copying in a lambda because this object gets deleted upon connection!
	auto* ctx = context;
	[ctx, str]() {
		ctx->connect();
		ctx->sendNoCheck(str);
	}();
}

std::string DisconnectedSocketState::getName() {
	return "Disconnected";
}

bool DisconnectedSocketState::connected() {
	return false;
}
