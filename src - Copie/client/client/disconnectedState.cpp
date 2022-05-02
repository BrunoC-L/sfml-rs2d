#include "disconnectedState.h"
#include "tcpSocket.h"

void DisconnectedSocketState::send(const std::string& str) {
	// copying in a lambda because the state gets deleted upon connection
	auto* ctx = &context;
	[ctx, str]() {
		if (ctx->connect())
			ctx->sendNoCheck(str);
	}();
}

std::string DisconnectedSocketState::getName() {
	return "Disconnected";
}

bool DisconnectedSocketState::connected() {
	return false;
}
