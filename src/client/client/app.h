#pragma once
#include <thread>

#include "mouseWheelEvent.h"
#include "../../common/common/event.h"
#include "VPixelToVTileConverter.h"
#include "mouseLeftClickEvent.h"
#include "mouseRightClickEvent.h"
#include "mouseMoveEvent.h"
#include "../../common/common/abstractServiceProvider.h"
#include <SFML/Network.hpp>

template <
    typename RenderWindow,
    typename Socket,
    typename Measures,
    typename Player,
    typename Camera,
    typename Map,
    typename Chat,
    typename Inventory,
    typename GameData
>
class App : public AbstractServiceProvider, public Service {
    AbstractRenderWindow* renderWindow;
public:
    App() : Service(this) {
        measures = new Measures(this);
        map = new Map(this);
        player = new Player(this);
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
        while (renderWindow->isOpen()) {
            renderWindow->events();
            renderWindow->draw();
        }
    }
};
