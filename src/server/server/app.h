#pragma once
#include <thread>
#include "json-socket-server.h"
#include "../../common/json.h"
#include "../../common/units.h"

template <
    typename Map,
    typename DB,
    typename UserService,
    typename PlayerActionService,
    typename SocketServerService
>
class App : public AbstractServiceProvider, private Service {
    std::thread gameTicks;
public:
    App(/* port, connectionstring */) : Service(this) {
        dbService = new DB(this, (wchar_t*)L"DRIVER=SQL Server Native Client 11.0;SERVER=DESKTOP-FJJ4HB5\\SQLEXPRESS;DATABASE=rs2d;Trusted_Connection=Yes;");
        map = new Map(this);
        userService = new UserService(this);
        playerActionService = new PlayerActionService(this);
        server = new SocketServerService(this, 38838);
    }

    void init() {
        dbService->init();
        map->init();
        userService->init();
        playerActionService->init();
        server->init();
    }

    void stop() {
        // server->stop();
        gameTicks.join();
    }

    void start() {
        bool stop = false;
        gameTicks = std::thread(
            [&]() {
                sf::Clock clock;
                while (!stop) {
                    if (clock.getElapsedTime().asMilliseconds() < 600)
                        continue;
                    clock.restart();
                    playerActionService->onGameTick();
                }
            }
        );

        while (!stop) {
            std::string action;
            std::cin >> action;
            if (action == "stop")
                stop = true;
            // send commands to executor
        }
    }
};
