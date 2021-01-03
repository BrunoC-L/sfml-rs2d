#pragma once
#include <SFML/Network.hpp>
#include "json.h"
#include "json-socket-server.h"

class JSONTCPPacket {
private:
	std::string str;
public:
	JSONTCPPacket(std::string type, JSON data) {
		str = "{'type':'" + type + "', 'data':" + data.asString() + "}";
	}
	void send(JSONTCPSocket* socket) {
		socket->send(str + messageEnd);
	}
};

class JSONTCPSocket {
	sf::TcpSocket* socket;
public:
	void send(std::string str) {
		socket->send(str.c_str(), str.length());
	}
};
