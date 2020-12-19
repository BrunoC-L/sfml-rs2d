#pragma once
#include "measures.h"
#include "getRenderWindow.h"
#include "chatMessage.h"

using namespace std;
using namespace sf;

class Chat {
public:
	void draw();
	void addMessage(ChatMessage message);
	vector<ChatMessage> messages;
	vector<ChatMessage> getFilteredMessagesList(int filter);
};
