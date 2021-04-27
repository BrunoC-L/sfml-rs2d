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
        std::cout << "Main Thread: " << std::this_thread::get_id() << std::endl;
        acquire();
        std::cout << "Initializing App\n";
        dbService->init();
        map->init();
        userService->init();
        playerActionService->init();
        server->init();
        scheduler->init();
        std::cout << "App Initialized\n";
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
                std::cout << "Tick Thread: " << std::this_thread::get_id() << std::endl;
                while (!stop) {
                    try {
                        if (tickScheduler->shouldTick())
                            TickEvent().emit();
                    }
                    catch (std::exception e) {
                        std::cout << "Tick Thread Error: " << e.what() << std::endl;
                    }
                }
            }
        );

        std::cout << "Console Read Thread: " << std::this_thread::get_id() << std::endl;
        while (!stop) {
            try {
                std::string action;
                std::cin >> action;
                if (action == "stop")
                    stop = true;
                // other commands here...
            }
            catch (std::exception e) {
                std::cout << "Console Read Thread Error: " << e.what() << std::endl;
            }
        }
    }
};
