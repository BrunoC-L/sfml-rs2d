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
    typename SocketServerService,
    typename TickScheduler
>
class App : public AbstractServiceProvider, private Service {
    std::thread gameTicks;
public:
    App(unsigned port, wchar_t* connectionString) : Service(this) {
        dbService = new DB(this, connectionString);
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
                TickScheduler tickScheduler;
                while (!stop) {
                    if (tickScheduler.shouldTick())
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
