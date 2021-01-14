#pragma once
#include <SFML/Network.hpp>
#include "../../common/common/json.h"

const std::string messageEnd = "|END|";

class JSONTCPSocket {
public:
	sf::TcpSocket* socket;
	void connect(std::string ip, unsigned port) {
		if (socket)
			disconnect();
		socket = new sf::TcpSocket();
		const auto status = socket->connect(ip, port);
		if (status != sf::Socket::Done)
			throw std::exception("Could not connect to the server\n");
		std::cout << "Connected to " << ip << ':' << port << std::endl;
	}

	void disconnect() {
		socket->disconnect();
		delete socket;
	}

	void send(std::string str) {
		socket->send(str.c_str(), str.length());
	}
};
