#include "socket.h"
#include "login.h"
#include "keyPressedEvent.h"

Socket::Socket(AbstractServiceProvider* provider) : Service(provider) {
	provider->set("Socket", this);
}

void Socket::init() {
	acquire();
    connect("127.0.0.1", 38838);
	listener = std::thread(
		[&]() {
            login();
            std::string buffer = "";
            while (true) {
                char data[1024] = { 0 };
                std::size_t received;
                sf::Socket::Status status;
                status = socket.socket->receive(data, 1024, received);
                if (status == sf::Socket::Status::Disconnected)
                     throw std::exception();
                if (!status == sf::Socket::Status::Done)
                    throw std::exception();

                buffer += std::string(data).substr(0, received);
                int index = 0;

                while ((index = buffer.find(messageEnd)) != -1) {
                    std::string str(buffer.substr(0, index));
                    try {
                        JSON json(str);
                        receive(json["type"].asString(), json["data"]);
                    } catch (...) { }
                    buffer = buffer.substr(index + messageEnd.length());
                }
            }
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
	socket.send(json.asString() + messageEnd);
}

void Socket::on(std::string type, std::function<void(JSON&)> callback) {
	callbacks[type].push_back(callback);
}

void Socket::login() {
    on("login",
        [&](JSON& data) {
            LoginEvent(data).emit();
        }
    );

    on("salts",
        [&](JSON& data) {
            player->setSalts(data["tempsalt"].asString(), data["permsalt"].asString());
            player->login();
        }
    );
}

void Socket::connect(std::string ip, unsigned port) {
    socket.connect(ip, port);
}

void Socket::disconnect() {
    socket.disconnect();
}
