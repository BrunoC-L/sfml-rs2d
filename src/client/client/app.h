#pragma once
#include <thread>

#include "mouseWheelEvent.h"
#include "event.h"
#include "VPixelToVTileConverter.h"
#include "mouseLeftClickEvent.h"
#include "mouseRightClickEvent.h"
#include "mouseMoveEvent.h"
#include "abstractServices.h"
#include "serviceProvider.h"

class App : public Service {
public:
    AbstractRenderWindow* renderWindow;
    bool stopping = false;

    App(
        ServiceProvider* provider,
        AbstractRenderWindow* window
    ) : renderWindow(window),
        Service(provider) {
    }

    void init() {
        acquire();
        measures->init();
        player->init();
        camera->init();
        map->init();
        chat->init();
        inventory->init();
        gameData->init();
        socket->init();
        renderWindow->init();
    }

    void stop(bool closeWindow = false) {
        stopping = true;
        map->stopUpdates();
        if (closeWindow)
            renderWindow->close();
        socket->disconnect();
    }

	void start() {
        while (renderWindow->isOpen() && !stopping) {
            renderWindow->events();
            renderWindow->draw();
        }
    }
};
