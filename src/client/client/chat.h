#pragma once
#include "chatMessage.h"
#include "abstractServices.h"
#include "service.h"

using namespace std;

class Chat : public AbstractChat, public Service {
public:
	Chat(AbstractServiceProvider* provider) : Service(provider) {
		provider->set("Chat", this);
	}
	void init() {
		acquire();
	}
	Chat() = default;
	void draw();
	void addMessage(ChatMessage message);
	vector<ChatMessage> messages;
	vector<ChatMessage> getFilteredMessagesList(int filter);
};
