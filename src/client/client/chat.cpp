#include "chat.h"

void Chat::addMessage(ChatMessage m) {
	messages.push_back(m);
}

std::vector<ChatMessage> Chat::getFilteredMessagesList(int filter) {
	std::vector<ChatMessage> filteredList = {};
	for (const auto& m : messages)
		if (m.type & filter)
			filteredList.push_back(m);
	return filteredList;
}
