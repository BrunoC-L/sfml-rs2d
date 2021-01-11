#pragma once
#include <thread>

#include "../../common/json.h"

#include "../../common/units.h"
#include "taskManager.h"
#include "mouseWheelEvent.h"
#include "../../common/event.h"
#include "VPixelToVTileConverter.h"
#include "mouseLeftClickEvent.h"
#include "mouseRightClickEvent.h"
#include "mouseMoveEvent.h"
#include "../../common/abstractServiceProvider.h"
#include <SFML/Network.hpp>

template <
    typename Measures,
    typename RenderWindow,
    typename TaskManager,
    typename Player,
    typename Camera,
    typename Map,
    typename Chat,
    typename Inventory,
    typename Socket,
    typename GameData
>
class App : public AbstractServiceProvider, public Service {
    AbstractRenderWindow* renderWindow;
public:
    App() : Service(this) {
        measures = new Measures(this);
        map = new Map(this);
        player = new Player(this);
        taskManager = new TaskManager(this);
        camera = new Camera(this);
        chat = new Chat(this);
        inventory = new Inventory(this);
        gameData = new GameData(this);
        socket = new Socket(this);
        renderWindow = new RenderWindow(this);
    }

    void init() {
        measures->init();
        player->init();
        camera->init();
        map->init();
        taskManager->init();
        chat->init();
        inventory->init();
        gameData->init();
        socket->init();
        renderWindow->init();
    }

    void stop() {
        map->stopUpdates();
    }

	void start() {
        unsigned frame = 0;
        while (renderWindow->isOpen()) {
            ++frame;

            renderWindow->events();
            renderWindow->update();
            player->update(frame % 60);
            renderWindow->clear();

            renderWindow->draw();
            renderWindow->display();
        }
    }
};
