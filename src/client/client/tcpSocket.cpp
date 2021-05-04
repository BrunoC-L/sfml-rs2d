#include "tcpSocket.h"
#include <iostream>
#include "disconnectedState.h"
#include "connectedState.h"

TCPSocket::TCPSocket(
    std::string ip,
    unsigned port,
    std::function<void()> onDisconnect,
    std::function<void(const std::string&)> onMessage
) : ip(ip),
    port(port),
    onDisconnect(onDisconnect),
    onMessage(onMessage)
{
    state = std::make_shared<DisconnectedSocketState>(*this);
}

bool TCPSocket::connect() {
	if (socket)
		disconnect();
	socket = std::make_unique<sf::TcpSocket>();
	auto status = socket->connect(ip, port);
	if (status != sf::Socket::Done) {
		std::cout << "Could not connect to the server\n";
		return false;
	}
	std::cout << "Connected to " << ip << ':' << port << std::endl;
    state = std::make_shared<ConnectedSocketState>(*this);
    if (listener)
        listener->join();
    listener = std::make_shared<std::thread>(
        [&]() {
            std::string buffer = "";
            while (state->connected()) {
                char data[1024] = { 0 };
                std::size_t received;
                sf::Socket::Status status = socket->receive(data, 1024, received);
                if (status != sf::Socket::Status::Done) {
                    disconnect();
                    return;
                }

                buffer += std::string(data).substr(0, received);
                int index = 0;

                while ((index = buffer.find(messageEnd)) != -1) {
                    onMessage(buffer.substr(0, index));
                    buffer.erase(buffer.begin(), buffer.begin() + index + messageEnd.length());
                }
            }
        }
    );
    return true;
}

void TCPSocket::disconnect() {
    if (!state->connected())
        return;
    state = std::make_shared<DisconnectedSocketState>(*this);
	socket->disconnect();
	socket.reset();
    onDisconnect();
}

void TCPSocket::sendNoCheck(const std::string& str) {
    auto msg = std::string(str) + messageEnd;
    auto s = socket->send(msg.c_str(), msg.length());
}

void TCPSocket::send(const std::string& str) {
    state->send(str);
}
