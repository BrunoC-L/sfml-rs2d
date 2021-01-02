#include "json-socket-server.h"
#include <chrono>

void createClientAndSendMessageOverSocket(unsigned port) {
    sf::TcpSocket socket;
    sf::Socket::Status status = socket.connect("127.0.0.1", port);
    if (status != sf::Socket::Done)
        throw std::exception("Could not connect to the server\n");
    std::cout << "Connected\n";

    char data[100] = { 0 };
    std::size_t received;
    sf::Socket::Status socketStatus = socket.receive(data, 100, received);
    if (socketStatus == sf::Socket::Disconnected)
        throw std::exception("Client was disconnected by the server\n");
    if (socketStatus != sf::Socket::Done)
        std::cout << "Error receiving data\n";
    else
        std::cout << "Client received \"" << data << "\" on connection\n";

    std::string str1("{'type':'hello', 'data': 'Hello server this is client'}|E");
    std::string str2("ND|");
    std::string str3("{'type':'hello', 'dat");
    std::string str4("a': 'Hello server this is client'}|END|");

    std::string badJson("a': 'Hello server this is client'}|END|");
    socket.send(badJson.c_str(), badJson.length());

    for (int i = 0; i < 5; ++i) {
        socket.send(str1.c_str(), str1.length());
        socket.send(str2.c_str(), str2.length());
        socket.send(str3.c_str(), str3.length());
        socket.send(str4.c_str(), str4.length());
    }

    JSON json;
    json["type"] = "login";
    JSON jsondata;
    jsondata["username"] = "'Bruno'";
    jsondata["password"] = "1234";
    json["data"] = jsondata;
    std::string login1(json.asString() + "|END|");
    jsondata["password"] = " 1324";
    json["data"] = jsondata;
    std::string login2(json.asString() + "|END|");
    jsondata["password"] = "1234";
    jsondata["username"] = "'Russian Spy!'";
    json["data"] = jsondata;
    std::string login3(json.asString() + "|END|");

    socket.send(login1.c_str(), login1.length());
    socket.send(login2.c_str(), login2.length());
    socket.send(login3.c_str(), login3.length());

    std::this_thread::sleep_for(std::chrono::seconds(1));
    socket.disconnect();
}

int main() {
    unsigned port = 38838;
    auto onError = [&](std::exception e, QueueMessage qm) {
        std::cout << e.what();
    };
    auto onDisconnect = [&](sf::TcpSocket* socket) {
        std::cout << socket << " disconnected\n";
    };

    JsonSocketServer socketServer(port, "|END|", onError, onDisconnect);

    auto onHello = [&](sf::TcpSocket* socket, JSON json) {
        std::cout <<"Server received \"" << json.asString() << "\"\n";
    };

    auto onLogin = [&](sf::TcpSocket* socket, JSON json) {
        if (json["username"].asString() == "Bruno" && json["password"].asString() == "1234")
            std::cout << "Bruno logged in!\n";
        else
            std::cout << "An imposter is trying to hack into the mainframe, sir!\n";
    };

    auto onPosition = [&](sf::TcpSocket* socket, JSON json) {
        for (int i = 0; i < socketServer.server.sockets.size(); ++i) {
            const auto& socket = *socketServer.server.sockets[i];
            JSON msg;
            msg["type"] = "position";
            msg["data"] = json;
            std::string str = msg.asString() + "|END|";
            socket.socket->send(str.c_str(), str.length());
        }
    };

    socketServer.on("hello", onHello);
    socketServer.on("login", onLogin);
    socketServer.on("position", onPosition);
    socketServer.start();

    std::thread client = std::thread(
        [&]() {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            createClientAndSendMessageOverSocket(port);
        }
    );

    bool stop = false;
    while (!stop) {
        std::string action;
        std::cin >> action;
        if (action == "stop")
            stop = true;
    }
    socketServer.stop();
    client.join();
}
