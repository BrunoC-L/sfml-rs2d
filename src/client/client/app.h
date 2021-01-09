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
public:
    App() : Service(this) {
        measures = new Measures(this);
        map = new Map(this);
        player = new Player(this);
        taskManager = new TaskManager(this);
        camera = new Camera(this);
        chat = new Chat(this);
        inventory = new Inventory(this);
        renderWindow = new RenderWindow(this);
        gameData = new GameData(this);
        socket = new Socket(this);
    }

    void init() {
        measures->init();
        player->init();
        camera->init();
        map->init();
        taskManager->init();
        chat->init();
        inventory->init();
        renderWindow->init();
        gameData->init();
        socket->init();
    }

    void stop() {
        map->stopUpdates();
    }

	void start() {
        unsigned frame = 0;
        unsigned tickmod = 0;
        // bool isGameTick = false;
        sf::Clock clock;

        while (renderWindow->isOpen()) {
            auto dt = clock.getElapsedTime().asMilliseconds();
            if (dt > 1100.f / 60)
                cout << "frame took " << dt << " ms" << endl;
            clock.restart();
            ++frame;
            tickmod = frame % unsigned(Measures::framesPerTick);
            // isGameTick = !tickmod;

            renderWindow->events();
            renderWindow->update();
            player->update(tickmod);
            renderWindow->clear();
            map->draw();
            player->draw();

            for (int i = 0; i < gameData->playerPositions.size(); ++i) {
                if (i == player->id)
                    continue;
                auto pos = gameData->playerPositions[i];
                renderWindow->draw(pos, 0, player->playerSprite);
            }

            renderWindow->draw();
            renderWindow->display();
        }
    }
};
