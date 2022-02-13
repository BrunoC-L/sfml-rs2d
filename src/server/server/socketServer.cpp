#include "socketServer.h"
#include "logoutEvent.h"

SocketServerService::SocketServerService(ServiceProvider* provider, unsigned port) : Service(provider) {
    provider->set(SERVER, this);
    auto onError = [&](std::exception& e, std::shared_ptr<Socket> socket) {
        std::cout << e.what() << std::endl;
    };

    auto onDisconnect = [&](std::shared_ptr<Socket> socket) {
        std::shared_ptr<User> user = socketToUser[socket];
        if (user->isLoggedIn)
            LogoutEvent(user).emit();
        socketToUser.erase(socket);
        userToSocket.erase(user);
        std::cout << socket << " disconnected\n";
    };

    auto onConnect = [&](std::shared_ptr<Socket> socket) {
        std::cout << socket << " connected\n";
        auto user = std::make_shared<User>();
        socketToUser[socket] = user;
        userToSocket[user] = socket;
    };

    std::cout << "port for socket server: " << port << "\n";
    socketServer    = std::make_unique<JSONSFMLSocketServer>   (port, onError, onConnect, onDisconnect);
    std::cout << "port for websocket server: " << port + 1 << "\n";
    webSocketServer = std::make_unique<JSONWebSocketServer>(port + 1, onError, onConnect, onDisconnect);
}

void SocketServerService::on(std::string msgType, std::function<void(std::shared_ptr<User>, const JSON&)> callback, bool loggedInRequired) {
    auto callback2 = [&, callback, loggedInRequired](std::shared_ptr<Socket> socket, const JSON& json) {
        auto user = socketToUser[socket];
        if (!user)
            return; // terrible patch
        if ((user->isLoggedIn && loggedInRequired) || !loggedInRequired)
            callback(user, json);
        else
            socket->disconnect();
    };
    socketServer->on(
        msgType,
        callback2
    );
    webSocketServer->on(
        msgType,
        callback2
    );
}

//void SocketServerService::send(std::shared_ptr<User> user, const JSON& msg) {
//    auto socket = userToSocket[user];
//    if (!socket)
//        return;
//    auto str = msg.asString() + messageEnd;
//    socket->send(str);
//}

void SocketServerService::send(std::shared_ptr<User> user, std::string type, const JSON& data) {
    auto socket = userToSocket[user];
    if(!socket) return;
    auto str = "{\"type\": \"" + type + "\", \"data\": " + data.asString() + "}";
    socket->send(str);
}

void SocketServerService::init() {
    acquire();
    socketServer->start();
    webSocketServer->start();
}

void SocketServerService::stop() {
    server->stop();
}
