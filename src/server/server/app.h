#pragma once
#include <thread>

#include "../../common/json.h"
#include "../../common/units.h"
#include "odbc.h"

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
        std::mutex mutex;
        std::vector<Query> queries;

        WCHAR* connectionString = (WCHAR*)L"DRIVER=SQL Server Native Client 11.0;SERVER=DESKTOP-FJJ4HB5\\SQLEXPRESS;DATABASE=rs2d;Trusted_Connection=Yes;";
        
        std::thread t([&]() { db(connectionString, queries, mutex); });
        
        queries.push_back(
            std::make_pair(
                "select * from Player\n",
                [&](QueryResult qr) {
                    for (auto r : qr) {
                        for (auto v : r)
                            std::cout << v << ", ";
                        std::cout << std::endl;
                    }
                }
            )
        );
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

        std::unordered_map<sf::TcpSocket*, std::string> socketToPlayerName;
        std::vector<sf::TcpSocket*> sockets(10, nullptr);
        

        std::vector<std::vector<VTile>> paths(10);
        std::vector<VTile> positions(10, VTile());

        auto onLogin = [&](sf::TcpSocket* socket, JSON json) {
            std::string username = json["username"].asString();
            int id = json["id"].asInt();
            socketToPlayerName[socket] = username;
            queries.push_back(
                std::make_pair(
                    "select * from Player where username = '" + username + "'\n",
                    [&, id, socket](QueryResult qr) {
                        auto res = qr[0];
                        int posx = atoi(res[4].c_str());
                        int posy = atoi(res[5].c_str());
                        positions[id] = VTile(posx, posy);
                        sockets[id] = socket;
                    }
                )
            );
        };


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
                        std::string q = "update Player set posx = " +
                            std::to_string(positions[i].x) +
                            ", posy = " +
                            std::to_string(positions[i].y) +
                            " where username = '" + socketToPlayerName[sockets[i]] + "'\n";
                        queries.push_back(
                            std::make_pair(
                                q,
                                [](QueryResult qr) { }
                            )
                        );
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

        //std::thread client = std::thread(
        //    [&]() {
        //        std::this_thread::sleep_for(std::chrono::seconds(1));
        //        createClientAndSendMessageOverSocket(port);
        //    }
        //);

        while (!stop) {
            std::string action;
            std::cin >> action;
            if (action == "stop")
                stop = true;
        }
        socketServer.stop();
        gameTicks.join();
        //client.join();
    }

    void stop() {}

    void init() {
         this->get("Map")->init();
    }
};
