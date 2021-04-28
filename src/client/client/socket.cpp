#include "socket.h"
#include "login.h"
#include "keyPressedEvent.h"
#include "disconnectedState.h"
#include "connectedState.h"

Socket::Socket(ServiceProvider* provider, std::string ip, int port) : Service(provider), ip(ip), port(port) {
	provider->set("Socket", this);
    state = std::make_shared<DisconnectedSocketState>(this);
}

void Socket::init() {
	acquire();

    on("login",
        [&](JSON& data) {
            LoginEvent(data).emit();
        }
    );

    on("salts",
        [&](JSON& data) {
            player->login(data["tempsalt"].asString(), data["permsalt"].asString());
        }
    );
}

void Socket::receive(std::string type, JSON& data) {
    for (auto lambda : callbacks[type])
        lambda(data);
}

void Socket::emit(std::string type, JSON& data) {
    JSON json;
    json["type"] = "'" + type + "'";
    json["data"] = data;
    send(json);
}

void Socket::send(JSON& json) {
    send(json.asString());
}

void Socket::on(std::string type, std::function<void(JSON&)> callback) {
	callbacks[type].push_back(callback);
}

bool Socket::connect() {
    if (!socket.connect(ip, port))
        return false;
    state = std::make_shared<ConnectedSocketState>(this);
    if (listener) {
        listener->join();
        delete listener;
        listener = nullptr;
    }
    listener = new std::thread(
        [&]() {
            std::string buffer = "";
            while (state->connected()) {
                char data[1024] = { 0 };
                std::size_t received;
                sf::Socket::Status status = socket.socket->receive(data, 1024, received);
                if (status != sf::Socket::Status::Done) {
                    disconnect();
                    return;
                }

                buffer += std::string(data).substr(0, received);
                int index = 0;

                while ((index = buffer.find(messageEnd)) != -1) {
                    std::string str(buffer.substr(0, index));
                    try {
                        JSON json(str);
                        receive(json["type"].asString(), json["data"]);
                    }
                    catch (...) {
                        std::cout << "Failed to create JSON from server message\n";
                    }
                    buffer = buffer.substr(index + messageEnd.length());
                }
            }
        }
    );
    return true;
}

void Socket::disconnect() {
    if (!state->connected())
        return;
    state = std::make_shared<DisconnectedSocketState>(this);
    socket.disconnect();
    LogoutEvent().emit();
}

void Socket::send(std::string str) {
    state->send(str);
}

void Socket::sendNoCheck(std::string str) {
    socket.send(str + messageEnd);
}
