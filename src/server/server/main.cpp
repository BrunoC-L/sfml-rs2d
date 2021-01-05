#include "json-socket-server.h"
#include <sstream>
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
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::string str2("ND|");
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::string str3("{'type':'hello', 'dat");
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
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

struct position {
    double x;
    double y;
};

int main() {
    unsigned port = 38838;

    auto onError = [&](std::exception& e, QueueMessage qm) {
        std::cout << e.what();
    };

    auto onDisconnect = [&](sf::TcpSocket* socket) {
        std::cout << socket << " disconnected\n";
    };
    int id = 0;
    auto onConnect = [&](sf::TcpSocket* socket) {
        std::string hello = std::to_string(id++);
        JSON json;
        json["type"] = "'hello'";
        json["data"] = "'" + hello + "'";
        std::string msg = json.asString();
        socket->send(msg.c_str(), msg.length());
        std::cout << socket << " connected\n";
    };

    JsonSocketServer socketServer(port, onError, onConnect, onDisconnect);

    auto onHello = [&](sf::TcpSocket* socket, JSON json) {
        std::cout <<"Server received \"" << json.asString() << "\"\n";
    };

    auto onLogin = [&](sf::TcpSocket* socket, JSON json) {
        if (json["username"].asString() == "Bruno" && json["password"].asString() == "1234")
            std::cout << "Bruno logged in!\n";
        else
            std::cout << "An imposter is trying to hack into the mainframe, sir!\n";
    };
    std::vector<position> positions(10);

    auto onPosition = [&](sf::TcpSocket* socket, JSON data) {
        positions[data["id"].asInt()] = { data["x"].asDouble(), data["y"].asDouble() };
    };

    socketServer.on("hello", onHello);
    socketServer.on("login", onLogin);
    socketServer.on("position", onPosition);
    socketServer.start();

    bool stop = false;
    std::thread gameTicks(
        [&]() {
            sf::Clock clock;
            while (!stop) {
                if (clock.getElapsedTime().asMilliseconds() < 600)
                    continue;
                clock.restart();
                for (int i = 0; i < positions.size(); ++i) {
                    JSON msg;
                    msg["type"] = "'position'";
                    msg["data"] = JSON();
                    msg["data"]["x"] = std::to_string(positions[i].x);
                    msg["data"]["y"] = std::to_string(positions[i].y);
                    msg["data"]["id"] = std::to_string(i);
                    std::string str = msg.asString() + messageEnd;
                    for (int j = 0; j < socketServer.server.sockets.size(); ++j) {
                        const auto& socket = *socketServer.server.sockets[j];
                        socket.socket->send(str.c_str(), str.length());
                    }
                }
            }
        }
    );

    std::thread client = std::thread(
        [&]() {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            createClientAndSendMessageOverSocket(port);
        }
    );

    while (!stop) {
        std::string action;
        std::cin >> action;
        if (action == "stop")
            stop = true;
    }
    socketServer.stop();
    gameTicks.join();
    client.join();
}
