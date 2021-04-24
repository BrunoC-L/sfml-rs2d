#pragma once
#include <thread>
#include "json-socket-server.h"
#include "json.h"
#include "units.h"
#include "abstractServices.h"
#include "tickScheduler.h"

class App : public ServiceProvider, private Service {
    std::thread gameTicks;
    TickScheduler* scheduler;
public:
    App(
        ServiceProvider* provider,
        AbstractSocketServer* server,
        AbstractDB* dbService,
        AbstractMap* map,
        AbstractPlayerActionService* playerActionService,
        AbstractUserService* userService,
        TickScheduler* scheduler
    ) : Service(provider), scheduler(scheduler) {

        // clearAllEventSubscribers();
    }

    void init() {
        acquire();
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
                while (!stop) {
                    if (scheduler->shouldTick())
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
