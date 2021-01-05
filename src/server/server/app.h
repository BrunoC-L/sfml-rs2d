#pragma once
#include <thread>

#include "../../common/json.h"
#include "../../common/units.h"

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
    int x;
    int y;
};

template <
    typename Map
>
class App : public AbstractServiceProvider {
    Map* map;
public:
    App() {
        map = new Map(this);
    }

    void start() {
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
            std::string msg = json.asString() + messageEnd;
            socket->send(msg.c_str(), msg.length());
            std::cout << socket << " connected\n";
        };

        JsonSocketServer socketServer(port, onError, onConnect, onDisconnect);

        auto onHello = [&](sf::TcpSocket* socket, JSON json) {
            std::cout << "Server received \"" << json.asString() << "\"\n";
        };

        auto onLogin = [&](sf::TcpSocket* socket, JSON json) {
            if (json["username"].asString() == "Bruno" && json["password"].asString() == "1234")
                std::cout << "Bruno logged in!\n";
            else
                std::cout << "An imposter is trying to hack into the mainframe, sir!\n";
        };
        std::vector<std::vector<VTile>> paths(10);
        std::vector<VTile> positions(10, VTile());

        auto onWalk = [&](sf::TcpSocket* socket, JSON data) {
            int id = data["id"].asInt();
            auto p1 = positions[id];
            int x2 = data["x"].asInt();
            int y2 = data["y"].asInt();
            std::cout << p1.x << ", " << p1.y << " to " << x2 << ", " << y2 << std::endl;
            if (p1.x)
                paths[id] = Pathfinder::pathfind(VTile(p1.x, p1.y), { VTile(x2, y2) }, false, map);
            else
                paths[id] = { VTile(x2, y2) };
        };

        socketServer.on("hello", onHello);
        socketServer.on("login", onLogin);
        socketServer.on("walk", onWalk);
        socketServer.start();

        bool stop = false;
        std::thread gameTicks(
            [&]() {
                sf::Clock clock;
                while (!stop) {
                    if (clock.getElapsedTime().asMilliseconds() < 600)
                        continue;
                    clock.restart();

                    // for every chunk
                    //   store chunk data to send
                    // for every player
                    //   send data for each chunk he sees
                    //   send personal data

                    JSON msg;
                    msg["type"] = "'positions'";
                    msg["data"] = "[]";

                    for (int i = 0; i < paths.size(); ++i) {
                        if (paths[i].size() == 0)
                            continue;
                        positions[i] = paths[i][0];
                        paths[i].erase(paths[i].begin());
                    }

                    for (int i = 0; i < positions.size(); ++i) {
                        JSON pos;
                        pos["x"] = std::to_string(positions[i].x);
                        pos["y"] = std::to_string(positions[i].y);
                        pos["id"] = std::to_string(i);
                        msg["data"].push(pos);
                    }

                    std::string str = msg.asString() + messageEnd;
                    for (int j = 0; j < socketServer.server.sockets.size(); ++j) {
                        const auto& socket = *socketServer.server.sockets[j];
                        socket.socket->send(str.c_str(), str.length());
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

    void stop() {}

    void init() {
        this->get("Map")->init();
    }
};
