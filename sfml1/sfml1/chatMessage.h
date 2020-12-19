#pragma once
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
using namespace std;

class ChatMessage {
public:
	ChatMessage(vector<pair<string, sf::Color>> parts) : parts(parts) {}
	ChatMessage operator+=(const ChatMessage& other) { parts.insert(parts.end(), other.parts.begin(), other.parts.end()); }
	vector<pair<string, sf::Color>> parts;
	int type = 0;
};
