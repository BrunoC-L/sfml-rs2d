#include "chat.h"

Chat::Chat(ServiceProvider* provider) : Service(provider) {
	provider->set(CHAT, this);
	messages.push_back(ChatMessage({ {"Hello World!", Color::BLUE } }, ChatMessageType::PlayerMessage));
	messages.push_back(ChatMessage({ {"This is PERFECT", Color::YELLOW } }, ChatMessageType::GameMessage));
}

void Chat::init() {
	acquire();
}

const std::vector<ChatMessage>& Chat::getMessages() {
	return messages;
}
