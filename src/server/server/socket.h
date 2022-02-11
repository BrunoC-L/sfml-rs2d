#pragma once
#include <SFML/Network.hpp>
#include <string>

enum class SocketStatus {
	DISCONNECTED, OK
};

class Socket {
public:
	virtual void send(const std::string& msg) = 0;
	virtual void disconnect() = 0;
	virtual sf::TcpSocket& getSFMLIMPL() = 0;
};
