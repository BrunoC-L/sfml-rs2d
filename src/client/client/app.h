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
#include "chunk.h"
#include "logger.h"
#include "session.h"

class App : public Service {
public:
    AbstractRenderWindow* renderWindow;
    bool stopping = false;
    CloseEvent::Observer closeObserver;

    App(
        ServiceProvider* provider,
        AbstractRenderWindow* window
    ) : renderWindow(window),
        Service(provider) {
    }

    void init() {
        closeObserver.set([&](const CloseEvent::Data& ev) {
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
        auto log = clientDefaultFolderLogger("app.txt", true);
        log("Starting frames");
        while (renderWindow->isOpen() && !stopping) {
            log("Frame events");
            renderWindow->events();
            log("Frame draw");
            renderWindow->draw();
        }
        log("Stopping frames");
    }
};
