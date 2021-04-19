#pragma once
#include <thread>

#include "mouseWheelEvent.h"
#include "event.h"
#include "VPixelToVTileConverter.h"
#include "mouseLeftClickEvent.h"
#include "mouseRightClickEvent.h"
#include "mouseMoveEvent.h"
#include "abstractServices.h"
#include "abstractServiceProvider.h"
#include "clearevents.h"

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

		clearAllEventSubscribers();
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
