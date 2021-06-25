#include "app.h"
#include "services.h"

void App::init() {
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
    resourceService->init();
    std::cout << "App Initialized\n";
}

void App::stop() {
    running = false;
    std::cout << "Stopping app\n";
    server->stop();
    dbService->stop();
}

void App::start() {
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
