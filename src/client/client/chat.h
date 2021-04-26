#pragma once
#include "chatMessage.h"
#include "abstractServices.h"
#include "service.h"

class Chat : public AbstractChat, public Service {
public:
	Chat(ServiceProvider* provider) : Service(provider) {
		provider->set("Chat", this);
	}
	void init() {
		acquire();
	}
	Chat() = default;
	void addMessage(ChatMessage message);
	std::vector<ChatMessage> messages;
	std::vector<ChatMessage> getFilteredMessagesList(int filter);
};
