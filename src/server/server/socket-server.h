#pragma once
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <vector>
#include <thread>
#include <unordered_map>
#include <iostream>
#include <functional>
#include <mutex>
#include "print.h"

const std::string messageEnd = "|END|";

struct SocketTuple {
    std::shared_ptr<sf::TcpSocket> socket;
    std::string buffer;
    std::mutex mutex;
};

class SocketServer {
private:
    sf::TcpListener listener;
    std::mutex selectorMutex;
    sf::SocketSelector selector;
    unsigned port;
    std::thread connectionThread;
    std::thread communicationThread;
    bool stopped = true;
    std::function<void(std::shared_ptr<sf::TcpSocket>, std::string)> onMessage;
    std::function<void(std::shared_ptr<sf::TcpSocket>)> onDisconnect;
    std::function<void(std::shared_ptr<sf::TcpSocket>)> onConnect;

    bool receive(std::shared_ptr<SocketTuple> socket) {
        std::lock_guard<std::mutex> guard(socket->mutex);
        const int maxSize = 1024;
        char buffer[maxSize] = { 0 };
        size_t receivedSize = 0;
        sf::Socket::Status socketStatus = socket->socket->receive(buffer, maxSize, receivedSize);
        if (socketStatus == sf::Socket::Disconnected)
            return false;
        socket->buffer += std::string(buffer).substr(0, receivedSize);
        int index = 0;
        while ((index = socket->buffer.find(messageEnd)) != -1) {
            onMessage(socket->socket, socket->buffer.substr(0, index));
            socket->buffer = socket->buffer.substr(index + messageEnd.length());
        }
        return true;
    }
public:
    std::vector<std::shared_ptr<SocketTuple>> sockets;
    SocketServer(
        unsigned port,
        std::function<void(std::shared_ptr<sf::TcpSocket>, std::string)> onMessage,
        std::function<void(std::shared_ptr<sf::TcpSocket>)> onConnect,
        std::function<void(std::shared_ptr<sf::TcpSocket>)> onDisconnect) :
    port(port), onMessage(onMessage), onConnect(onConnect), onDisconnect(onDisconnect) { }

    void start() {
        if (listener.listen(port) != sf::TcpSocket::Done)
            throw std::runtime_error("Could not listen to port " + std::to_string(port));

        stopped = false;

        connectionThread = std::thread(
            [&]() {
                {
                    std::ostringstream ss;
                    ss << "Connection Thread: " << std::this_thread::get_id() << std::endl;
                    print(ss);
                }
                int id = 0;
                while (!stopped) {
                    std::shared_ptr<sf::TcpSocket> client = std::make_shared<sf::TcpSocket>();
                    if (listener.accept(*client) != sf::Socket::Done)
                        continue;
                    std::lock_guard<std::mutex> lock(selectorMutex);
                    std::shared_ptr<SocketTuple> st = std::make_shared<SocketTuple>();
                    st->socket = client;
                    st->buffer = "";
                    sockets.push_back(st);
                    selector.add(*client);
                    onConnect(st->socket);
                }
                {
                    std::ostringstream ss;
                    ss << "Connection Thread " << std::this_thread::get_id() << " Exiting" << std::endl;
                    print(ss);
                }
            }
        );

        communicationThread = std::thread(
            [&]() {
                {
                    std::ostringstream ss;
                    ss << "Communication Thread: " << std::this_thread::get_id() << std::endl;
                    print(ss);
                }
                while (!stopped) {
                    if (sockets.size() == 0)
                        std::this_thread::sleep_for(std::chrono::milliseconds(10));
                    if (!selector.wait(sf::milliseconds(10))) // does not wait when there are no sockets
                        continue;
                    for (unsigned i = 0; i < sockets.size(); ++i) {
                        auto& socket = sockets[i];
                        if (!selector.isReady(*socket->socket))
                            continue;
                        if (receive(socket))
                            continue;
                        std::lock_guard<std::mutex> lock(selectorMutex);
                        onDisconnect(socket->socket);
                        selector.remove(*socket->socket);
                        sockets.erase(sockets.begin() + i);
                    }
                }
                {
                    std::ostringstream ss;
                    ss << "Communication Thread " << std::this_thread::get_id() << " Exiting" << std::endl;
                    print(ss);
                }
            }
        );
    }

    void stop() {
        stopped = true;
        listener.close();
        communicationThread.join();
        connectionThread.join();
    }
};
