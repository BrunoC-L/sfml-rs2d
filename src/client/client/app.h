#pragma once
#include <thread>

#include "mouseWheelEvent.h"
#include "../../common/common/event.h"
#include "VPixelToVTileConverter.h"
#include "mouseLeftClickEvent.h"
#include "mouseRightClickEvent.h"
#include "mouseMoveEvent.h"
#include "abstractServices.h"
#include "../../common/common/abstractServiceProvider.h"

class App : public Service {
public:
    AbstractRenderWindow* renderWindow;
    AbstractServiceProvider* provider;
    bool stopping = false;

    App(
        AbstractServiceProvider* provider,
        AbstractRenderWindow* window,
        AbstractSocket* socket,
        AbstractMeasures* measures,
        AbstractMap* map,
        AbstractPlayer* player,
        AbstractCamera* camera,
        AbstractChat* chat,
        AbstractInventory* inventory,
        AbstractGameDataService* gameData
    ) : renderWindow(window), Service(provider), provider(provider) {
        this->socket = socket;
        this->measures = measures;
        this->map = map;
        this->player = player;
        this->camera = camera;
        this->chat = chat;
        this->inventory = inventory;
        this->gameData = gameData;
    }

    void init() {
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
        map->stopUpdates();
        if (closeWindow)
            renderWindow->close();
        socket->disconnect();
        stopping = true;
    }

	void start() {
        while (renderWindow->isOpen() && !stopping) {
            if (!renderWindow->shouldFrame())
                continue;
            //if (renderWindow->shouldTick())
            renderWindow->events();
            renderWindow->draw();
        }
    }
};
