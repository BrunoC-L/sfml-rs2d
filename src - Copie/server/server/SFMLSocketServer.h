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
#include "socket.h"

constexpr auto messageEnd = "|END|";
constexpr size_t messageEndLen = 5;

class SFMLSocket : public Socket {
protected:
    std::unique_ptr<sf::TcpSocket> socket;
public:
    SFMLSocket(std::unique_ptr<sf::TcpSocket>&& socket) : socket(std::move(socket)) {}
    virtual void disconnect() override {
        socket->disconnect();
    }

    virtual void send(const std::string& msg) override {
        socket->send(msg.c_str(), msg.length());
        socket->send(messageEnd, messageEndLen);
    }

    sf::TcpSocket& getSFMLIMPL() {
        return *socket.get();
    }
};

struct SocketTuple {
    std::shared_ptr<Socket> socket;
    std::string buffer;
    std::mutex mutex;
};

class SFMLSocketServer {
private:
    sf::TcpListener listener;
    std::mutex selectorMutex;
    sf::SocketSelector selector;
    unsigned port;
    std::thread connectionThread;
    std::thread communicationThread;
    bool stopped = true;
    std::function<void(std::shared_ptr<Socket>, std::string)> onMessage;
    std::function<void(std::shared_ptr<Socket>)> onDisconnect;
    std::function<void(std::shared_ptr<Socket>)> onConnect;

    bool receive(std::shared_ptr<SocketTuple> socket) {
        std::lock_guard<std::mutex> guard(socket->mutex);
        const int maxSize = 1024;
        char buffer[maxSize] = { 0 };
        size_t receivedSize = 0;
        sf::Socket::Status socketStatus = dynamic_cast<SFMLSocket*>(socket->socket.get())->getSFMLIMPL().receive(buffer, maxSize, receivedSize);
        if (socketStatus == sf::Socket::Disconnected)
            return false;
        socket->buffer += std::string(buffer).substr(0, receivedSize);
        int index = 0;
        while ((index = socket->buffer.find(messageEnd)) != -1) {
            onMessage(socket->socket, socket->buffer.substr(0, index));
            socket->buffer = socket->buffer.substr(index + messageEndLen);
        }
        return true;
    }
public:
    std::vector<std::shared_ptr<SocketTuple>> sockets;
    SFMLSocketServer(
        unsigned port,
        std::function<void(std::shared_ptr<Socket>, std::string)> onMessage,
        std::function<void(std::shared_ptr<Socket>)> onConnect,
        std::function<void(std::shared_ptr<Socket>)> onDisconnect) :
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
                    std::unique_ptr<sf::TcpSocket> client = std::make_unique<sf::TcpSocket>();
                    if (listener.accept(*client.get()) != sf::Socket::Done)
                        continue;
                    std::lock_guard<std::mutex> lock(selectorMutex);
                    std::shared_ptr<SocketTuple> st = std::make_shared<SocketTuple>();
                    st->socket = std::make_shared<SFMLSocket>(std::move(client));
                    st->buffer = "";
                    sockets.push_back(st);
                    selector.add(dynamic_cast<SFMLSocket*>(st->socket.get())->getSFMLIMPL());
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
                        auto cast = dynamic_cast<SFMLSocket*>(socket->socket.get());
                        if (!selector.isReady(cast->getSFMLIMPL()))
                            continue;
                        if (receive(socket))
                            continue;
                        std::lock_guard<std::mutex> lock(selectorMutex);
                        onDisconnect(socket->socket);
                        selector.remove(cast->getSFMLIMPL());
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
