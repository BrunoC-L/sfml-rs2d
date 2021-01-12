#pragma once
#include <vector>
#include <string>
#include "color.h"
using namespace std;

enum ChatMessageType { PlayerMessage };

class ChatMessage {
public:
	ChatMessage(vector<pair<string, Color>> parts, ChatMessageType type) : parts(parts), type(type) {}
	vector<pair<string, Color>> parts;
	ChatMessageType type;
};
