#pragma once
#include <SFML/Network.hpp>
#include "json.h"

const std::string messageEnd = "|END|";

class JSONTCPSocket {
public:
	sf::TcpSocket* socket;
	bool connect(std::string ip, unsigned port) {
		if (socket)
			disconnect();
		socket = new sf::TcpSocket();
		const auto status = socket->connect(ip, port);
		if (status != sf::Socket::Done) {
			std::cout << "Could not connect to the server\n";
			return false;
		}
		std::cout << "Connected to " << ip << ':' << port << std::endl;
		return  true;
	}

	void disconnect() {
		socket->disconnect();
		delete socket;
		socket = nullptr;
	}

	void send(std::string str) {
		socket->send(str.c_str(), str.length());
	}
};
