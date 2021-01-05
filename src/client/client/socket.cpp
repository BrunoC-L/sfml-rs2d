#include "socket.h"

Socket::Socket(AbstractServiceProvider* provider) : Service(provider) {
	provider->set("Socket", this);
}

void Socket::init() {
	acquire();
	socket.connect("127.0.0.1", 38838);
    player->walk(player->position);
	listener = std::thread(
		[&]() {
            std::string buffer = "";
            while (true) {
                char data[1024] = { 0 };
                std::size_t received;
                if (socket.socket->receive(data, 1024, received) != sf::Socket::Done)
                    continue;

                buffer += std::string(data).substr(0, received);
                int index = 0;

                while ((index = buffer.find(messageEnd)) != -1) {
                    std::string str(buffer.substr(0, index));
                    try {
                        JSON json(str);
                        receive(json["type"].asString(), json["data"]);
                        //if (json["type"].asString() == "position") {
                        //    auto data = json["data"];
                        //    int otherid = data["id"].asInt();
                        //    int x = data["x"].asDouble();
                        //    int y = data["y"].asDouble();
                        //    positions[otherid] = VTile(x, y);
                        //}
                        //else if (json["type"].asString() == "hello") {
                        //    id = json["data"].asInt();
                        //}
                    } catch (...) { }
                    buffer = buffer.substr(index + messageEnd.length());
                }
            }
		}
	);
}

void Socket::receive(std::string type, JSON data) {
    const auto lambdas = callbacks[type];
    for (int i = 0; i < lambdas.size(); ++i) {
        lambdas[i](data);
    }
}

void Socket::emit(std::string type, JSON data) {
    this->socket.send("{'type':'" + type + "', 'data':" + data.asString() + "}" + messageEnd);
}

void Socket::send(JSON json) {
	socket.send(json.asString());
}

void Socket::on(std::string type, std::function<void(JSON)> callback) {
	callbacks[type].push_back(callback);
}
