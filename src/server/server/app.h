#pragma once
#include <thread>
#include "json-socket-server.h"
#include "json.h"
#include "units.h"
#include "abstractServices.h"
#include "tickScheduler.h"
#include "tickEvent.h"
#include "print.h"

class App : public ServiceProvider, private Service {
    TickScheduler* tickScheduler;
    bool running = false;
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
        std::ostringstream ss;
        ss << "Main Thread: " << std::this_thread::get_id() << std::endl;
        print(ss);
        acquire();
        std::cout << "Initializing App\n";
        dbService->init();
        map->init();
        server->init();
        scheduler->init();
        userService->init();
        playerActionService->init();
        std::cout << "App Initialized\n";
    }

    void stop() {
        running = false;
        std::cout << "Stopping app\n";
        playerActionService->stop();
        userService->stop();
        scheduler->stop();
        server->stop();
        map->stop();
        dbService->stop();
    }

    void start() {
        running = true;
        {
            std::ostringstream ss;
            ss << "Tick Thread: " << std::this_thread::get_id() << std::endl;
            print(ss);
        }
        while (running) {
            try {
                if (tickScheduler->shouldTick() && running)
                    TickEvent().emit();
            }
            catch (std::exception e) {
                std::cout << "Tick Thread Error: " << e.what() << std::endl;
            }
        }
        {
            std::ostringstream ss;
            ss << "Tick Thread " << std::this_thread::get_id() << " Exiting" << std::endl;
            print(ss);
        }
    }
};
