#pragma once
#include "chatMessage.h"
#include "abstractServices.h"

using namespace std;

class Chat : public AbstractChat {
public:
	Chat(AbstractServiceProvider* provider) {
		REGISTER(Chat);
	}
	void init() {
		ACQUIRE
	}
	Chat() = default;
	void draw();
	void addMessage(ChatMessage message);
	vector<ChatMessage> messages;
	vector<ChatMessage> getFilteredMessagesList(int filter);
};
