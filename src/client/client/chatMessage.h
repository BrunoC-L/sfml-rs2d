#pragma once
#include <vector>
#include <string>
#include "color.h"

enum class ChatMessageType { PlayerMessage, GameMessage };

class ChatMessage {
public:
	ChatMessage(std::vector<std::pair<std::string, Color>> parts, ChatMessageType type) : parts(parts), type(type) {}
	std::vector<std::pair<std::string, Color>> parts;
	ChatMessageType type;
};
