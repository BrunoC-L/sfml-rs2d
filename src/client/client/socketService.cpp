#include "socketService.h"
#include "login.h"
#include "keyPressedEvent.h"

Socket::Socket(ServiceProvider* provider, std::string ip, int port) : Service(provider), socket(ip, port, [&]() {onDisconnect(); }, [&](const std::string& msg) {onMessage(msg); }) {
    provider->set(SOCKET, this);
}

void Socket::onMessage(const std::string& message) {
    std::string type;
    std::shared_ptr<const JSON> data;
    try {
        JSON json;
        try {
            json = JSON(message);
            type = json.get("type").asString();
            data = std::make_shared<const JSON>(std::move(json["data"]));
            std::exchange(json["data"], {});
        }
        catch (...) {
            std::cout << "Failed to create JSON from server message\n";
        }
        receive(type, data);
    }
    catch (...) {
        std::cout << "Failed to take actions on reception of " << type << std::endl;
    }
}

void Socket::onDisconnect() {
    LogoutEvent().emit();
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

void Socket::send(const std::string& type, const JSON& data) {
    socket.send("{'type':'" + type + "', 'data':" + data.asString() + "}");
}

void Socket::send(const JSON& json) {
    socket.send(json.asString());
}

void Socket::on(const std::string& type, std::function<void(const std::shared_ptr<const JSON>&)> callback) {
	callbacks[type].push_back(callback);
}

bool Socket::connect() {
    return socket.connect();
}

void Socket::disconnect() {
    socket.disconnect();
}

void Socket::stop() {
    disconnect();
    socket.stop();
}
