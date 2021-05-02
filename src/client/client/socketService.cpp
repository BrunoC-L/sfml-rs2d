#include "socketService.h"
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
        [&](const std::shared_ptr<const JSON>& data) {
            LoginEvent(*data).emit();
        }
    );

    on("salts",
        [&](const std::shared_ptr<const JSON>& data) {
            player->login(data->get("tempsalt").asString(), data->get("permsalt").asString());
        }
    );
}

void Socket::receive(const std::string& type, std::shared_ptr<const JSON> data) {
    for (auto lambda : callbacks[type])
        lambda(data);
}

void Socket::emit(const std::string& type, const JSON& data) {
    JSON json;
    json["type"] = "'" + type + "'";
    json["data"] = data;
    send(json);
}

void Socket::send(const JSON& json) {
    send(json.asString());
}

void Socket::on(const std::string& type, std::function<void(const std::shared_ptr<const JSON>&)> callback) {
	callbacks[type].push_back(callback);
}

bool Socket::connect() {
    if (!socket.connect(ip, port))
        return false;
    state = std::make_shared<ConnectedSocketState>(this);
    if (listener)
        listener->join();
    listener = std::make_shared<std::thread>(
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
                    // move semantics for swapping json["data"] into jsonData
                    std::string str(buffer.substr(0, index));
                    std::string type;
                    std::shared_ptr<const JSON> data;
                    // the new JSON(); gets deleted by JSON json's destructor
                    JSON* jsonData = new JSON();
                    try {
                        JSON json;
                        try {
                            json = JSON(str);
                            type = json.get("type").asString();
                            std::swap(*jsonData, json["data"]);
                            data = std::shared_ptr<const JSON>(jsonData);
                        }
                        catch (...) {
                            std::cout << "Failed to create JSON from server message\n";
                        }
                        receive(type, data);
                    }
                    catch (...) {
                        std::cout << "Failed to take actions on reception of " << type << std::endl;
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

void Socket::send(const std::string& str) {
    state->send(str);
}

void Socket::sendNoCheck(const std::string& str) {
    socket.send(str + messageEnd);
}
