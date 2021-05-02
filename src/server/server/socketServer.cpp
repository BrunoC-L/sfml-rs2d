#include "socketServer.h"
#include "loginout.h"

SocketServerService::SocketServerService(ServiceProvider* provider, unsigned port) : Service(provider) {
    provider->set("Server", this);
    auto onError = [&](std::exception& e, QueueMessage qm) {
        std::cout << e.what() << std::endl;
    };

    auto onDisconnect = [&](std::shared_ptr<sf::TcpSocket> socket) {
        std::shared_ptr<User> user = socketToUser[socket];
        LogoutEvent(user).emit();
        socketToUser.erase(socket);
        userToSocket.erase(user);
        std::cout << socket << " disconnected\n";
    };

    auto onConnect = [&](std::shared_ptr<sf::TcpSocket> socket) {
        std::cout << socket << " connected\n";
        auto user = std::make_shared<User>();
        socketToUser[socket] = user;
        userToSocket[user] = socket;
        LoginEvent(socketToUser[socket]).emit();
    };

    server = std::make_unique<JsonSocketServer>(port, onError, onConnect, onDisconnect);
}

void SocketServerService::on(std::string msgType, std::function<void(std::shared_ptr<User>, JSON&)> callback, bool loggedInRequired) {
    server->on(
        msgType,
        [&, callback, loggedInRequired](std::shared_ptr<sf::TcpSocket> socket, JSON& json) {
            auto user = socketToUser[socket];
            if (user->isLoggedIn && loggedInRequired || !loggedInRequired)
                callback(user, json);
            else
                socket->disconnect();
        }
    );
}

void SocketServerService::send(std::shared_ptr<User> user, JSON& msg) {
    auto socket = userToSocket[user];
    auto str = msg.asString() + messageEnd;
    socket->send(str.c_str(), str.length());
}

void SocketServerService::send(std::shared_ptr<User> user, std::string type, JSON& data) {
    auto socket = userToSocket[user];
    auto str = "{'type': '" + type + "', 'data': " + data.asString() + "}" + messageEnd;
    socket->send(str.c_str(), str.length());
}

void SocketServerService::init() {
    acquire();
    server->start();
}

void SocketServerService::stop() {
    server->stop();
}
