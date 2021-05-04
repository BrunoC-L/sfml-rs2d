#pragma once
#include <SFML/Network.hpp>
#include "socketState.h"
#include <thread>
#include <vector>
#include <functional>
#include <unordered_map>

const std::string messageEnd = "|END|";

class TCPSocket {
private:
	std::string ip;
	unsigned port;
	std::shared_ptr<SocketState> state;
	std::shared_ptr<std::thread> listener;
	std::function<void()> onDisconnect;
	std::function<void(const std::string&)> onMessage;
public:
	std::unique_ptr<sf::TcpSocket> socket;
	TCPSocket(std::string ip, unsigned port, std::function<void()> onDisconnect, std::function<void(const std::string&)> onMessage);
	bool connect();
	void disconnect();
	void send(const std::string& str);
	void sendNoCheck(const std::string& message);
};
