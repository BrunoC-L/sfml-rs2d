#pragma once
#include <thread>
#include "json-socket-server.h"
#include "json.h"
#include "units.h"
#include "service.h"
#include "tickScheduler.h"
#include "tickEvent.h"
#include "print.h"
#include "service.h"

class App : private Service {
    TickScheduler* tickScheduler;
    bool running = false;
public:
    App(ServiceProvider* provider, TickScheduler* tickScheduler) : Service(provider), tickScheduler(tickScheduler) {}
    void init();
    void stop();
    void start();
};
