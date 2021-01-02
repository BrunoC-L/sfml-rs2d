#include "chat.h"

void Chat::draw() {

}

void Chat::addMessage(ChatMessage m) {
	messages.push_back(m);
}

vector<ChatMessage> Chat::getFilteredMessagesList(int filter) {
	vector<ChatMessage> filteredList = {};
	for (const auto& m : messages)
		if (m.type & filter)
			filteredList.push_back(m);
	return filteredList;
}
