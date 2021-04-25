#pragma once
#include <thread>
#include "json-socket-server.h"
#include "json.h"
#include "units.h"
#include "abstractServices.h"
#include "tickScheduler.h"
#include "tick.h"

class App : public ServiceProvider, private Service {
    std::thread gameTicks;
    TickScheduler* tickScheduler;
public:
    App(
        ServiceProvider* provider,
        AbstractSocketServer* server,
        AbstractDB* dbService,
        AbstractMap* map,
        AbstractPlayerActionService* playerActionService,
        AbstractUserService* userService,
        TickScheduler* tickScheduler,
        AbstractTaskScheduler* scheduler
    ) : Service(provider), tickScheduler(tickScheduler) {
        clearAllEventSubscribers();
    }

    void clearAllEventSubscribers() {
    }

    void init() {
        acquire();
        dbService->init();
        map->init();
        userService->init();
        playerActionService->init();
        server->init();
        scheduler->init();
    }

    void stop() {
        throw std::exception("Not implemented");
        //server->stop();
        gameTicks.join();
    }

    void start() {
        bool stop = false;
        gameTicks = std::thread(
            [&]() {
                while (!stop) {
                    if (tickScheduler->shouldTick())
                        TickEvent().emit();
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
