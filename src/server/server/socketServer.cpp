#include "socketServer.h"

SocketServerService::SocketServerService(AbstractServiceProvider* provider, unsigned port) : Service(provider) {
    provider->set("Server", this);
    auto onError = [&](std::exception& e, QueueMessage qm) {
        std::cout << e.what() << std::endl;
    };

    auto onDisconnect = [&](sf::TcpSocket* socket) {
        std::cout << socket << " disconnected\n";
    };

    int* id = new int;
    *id = 0;
    auto onConnect = [&, id](sf::TcpSocket* socket) {
        std::string hello = std::to_string(*id);
        JSON json;
        json["type"] = "'hello'";
        json["data"] = "'" + hello + "'";
        std::string msg = json.asString() + messageEnd;
        socket->send(msg.c_str(), msg.length());
        std::cout << socket << " connected\n";
        auto user = std::make_shared<User>(*id);
        socketToUser[socket] = user;
        userToSocket[user] = socket;
        users.push_back(user);
        ++*id;
    };

    server = new JsonSocketServer(port, onError, onConnect, onDisconnect);
}

void SocketServerService::on(std::string msgType, std::function<void(std::shared_ptr<User>, JSON)> callback) {
    server->on(
        msgType,
        [&, callback](sf::TcpSocket* socket, JSON json) {
            auto user = socketToUser[socket];
            callback(user, json);
        }
    );
}

void SocketServerService::send(std::shared_ptr<User> user, JSON msg) {
    auto socket = userToSocket[user];
    auto str = msg.asString() + messageEnd;
    socket->send(str.c_str(), str.length());
}

void SocketServerService::send(std::shared_ptr<User> user, std::string type, JSON data) {
    auto socket = userToSocket[user];
    auto str = "{'type': '" + type + "', 'data': " + data.asString() + "}" + messageEnd;
    socket->send(str.c_str(), str.length());
}

void SocketServerService::init() {
    acquire();
    auto onHello = [&](std::shared_ptr<User>, JSON json) {
        std::cout << "Server received \"" << json.asString() << "\"\n";
    };
    on("hello", onHello);
    server->start();
}
