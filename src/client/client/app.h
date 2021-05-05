#pragma once
#include <thread>

#include "mouseWheelEvent.h"
#include "VPixelToVTileConverter.h"
#include "mouseLeftClickEvent.h"
#include "mouseRightClickEvent.h"
#include "mouseMoveEvent.h"
#include "abstractServices.h"
#include "serviceProvider.h"
#include "closeEvent.h"
#include "print.h"

class App : public Service {
public:
    AbstractRenderWindow* renderWindow;
    bool stopping = false;
    EventObserver<CloseEvent> closeObserver;

    App(
        ServiceProvider* provider,
        AbstractRenderWindow* window
    ) : renderWindow(window),
        Service(provider) {
    }

    void init() {
        closeObserver.set([&](CloseEvent& ev) {
            {
                std::ostringstream ss;
                ss << "Stopping app" << std::endl;
                print(ss);
            }
            stop();
            {
                std::ostringstream ss;
                ss << "Stopped app" << std::endl;
                print(ss);
            }
        });

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
        socket->stop();
    }

	void start() {
        while (renderWindow->isOpen() && !stopping) {
            renderWindow->events();
            renderWindow->draw();
        }
    }
};
