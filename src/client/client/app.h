#pragma once
#include <thread>

#include "../../common/json.h"

#include "../../common/units.h"
#include "taskManager.h"
#include "mouseWheelEvent.h"
#include "event.h"
#include "VPixelToVTileConverter.h"
#include "mouseLeftClickEvent.h"
#include "mouseRightClickEvent.h"
#include "mouseMoveEvent.h"
#include "../../common/abstractServiceProvider.h"
#include "rightBanner.h"
#include "bottomBanner.h"
#include "rightClickInterface.h"
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
    typename Socket
>
class App : public AbstractServiceProvider {
    Measures* measures;
    Map* map;
    Player* player;
    TaskManager* taskManager;
    Camera* camera;
    Chat* chat;
    Inventory* inventory;
    RenderWindow* renderWindow;
    Socket* socket;
public:
    App() {
        measures = new Measures(this);
        map = new Map(this);
        player = new Player(this);
        taskManager = new TaskManager(this);
        camera = new Camera(this);
        chat = new Chat(this);
        inventory = new Inventory(this);
        renderWindow = new RenderWindow(this);
        socket = new Socket(this);
    }

    std::vector<VTile> positions = std::vector<VTile>(10);
	void start() {

        RightBanner rightBanner = RightBanner(this);
        BottomBanner bottomBanner = BottomBanner(this);
        RightClickInterface rightClickInterface = RightClickInterface(this);

        std::thread t(&Map::doUpdates, map);

        auto converter = VPixelToVTileConverter(this);

        converter.getPositionInGame(VPixel());

        renderWindow->setFramerateLimit(60);
        unsigned frame = 0;
        unsigned tickmod = 0;
        bool isGameTick = false;
        sf::Clock clock;

        bool clickedOnRightClickInterface = false;
        bool clickedOnRightBanner = false;
        bool clickedOnWorld = true;
        // bool clickedOnBottomBanner = false;

        // REMOVE LATER
        auto x = new EventObserver<MouseWheelEvent>([&](MouseWheelEvent* ev) {
            measures->zoom = measures->zoom * (1 + (0.3 + ev->delta) * 0.1f);
            });
        MouseWheelEvent::subscribe(x);

        auto x2 = new EventObserver<MouseLeftClickEvent>([&](MouseLeftClickEvent* ev) {
            clickedOnRightClickInterface = rightClickInterface.active && rightClickInterface.mouseIsInRect(ev);
            if (clickedOnRightClickInterface) {
                clickedOnWorld = false;
                rightClickInterface.click(ev);
            }
            clickedOnRightBanner = rightBanner.mouseIsInRect(ev);
            if (clickedOnRightBanner) {
                clickedOnWorld = false;
                rightBanner.click(ev);
            }
            if (clickedOnWorld) {
                VTile tileClicked = converter.getPositionInGame(ev->pos);
                Tile* t = map->getTileFromVTile(tileClicked);
                if (t)
                    t->onLeftClick(*ev);
            }
            });
        MouseLeftClickEvent::subscribe(x2);

        auto x3 = new EventObserver<MouseRightClickEvent>([&](MouseRightClickEvent* ev) {
            if (!rightClickInterface.active || !rightClickInterface.mouseIsInRect(ev)) {
                rightClickInterface.setPosition(ev->pos);
                VTile tileClicked = converter.getPositionInGame(ev->pos);
                Tile* t = map->getTileFromVTile(tileClicked);
                if (t)
                    t->onRightClick(*ev);
            }
            });
        MouseRightClickEvent::subscribe(x3);

        auto x4 = new EventObserver<MouseMiddleClickEvent>([&](MouseMiddleClickEvent* ev) {
            clickedOnRightClickInterface = rightClickInterface.active && rightClickInterface.mouseIsInRect(ev);
            if (clickedOnRightClickInterface)
                clickedOnWorld = false;
            clickedOnRightBanner = rightBanner.mouseIsInRect(ev);
            if (clickedOnRightBanner)
                clickedOnWorld = false;
            if (clickedOnWorld) {
                VTile tileClicked = converter.getPositionInGame(ev->pos);
                Tile* t = map->getTileFromVTile(tileClicked);
                if (t)
                    t->onMiddleClick(*ev);
            }
            });
        MouseMiddleClickEvent::subscribe(x4);

        auto x5 = new EventObserver<MouseMoveEvent>([&](MouseMoveEvent* ev) {
            // add top left indicator of what your mouse is over + left click option + options.length
            if (rightClickInterface.active && !rightClickInterface.mouseIsInRect(ev)) {
                rightClickInterface.active = false;
            }
            });
        MouseMoveEvent::subscribe(x5);

        while (renderWindow->isOpen()) {
            auto dt = clock.getElapsedTime().asMilliseconds();
            if (dt > 1100.f / 60)
                cout << "frame took " << dt << " ms" << endl;
            clock.restart();
            ++frame;
            tickmod = frame % unsigned(Measures::framesPerTick);
            isGameTick = !tickmod;
            if (isGameTick) {
                map->shouldUpdate = true;
            }

            sf::Event event;
            clickedOnRightClickInterface = false;
            clickedOnRightBanner = false;
            clickedOnWorld = true;
            // clickedOnBottomBanner = false;

            renderWindow->events();

            rightBanner.update();
            player->update(tickmod);
            renderWindow->clear();

            map->draw();
            player->draw();

            for (int i = 0; i < positions.size(); ++i) {
                if (i == player->id)
                    continue;
                auto pos = positions[i];
                if (!pos.x && !pos.y)
                    continue;
                renderWindow->draw(pos, 0, player->playerSprite);
            }

            bottomBanner.draw();
            rightBanner.draw();
            rightClickInterface.draw();

            renderWindow->display();
        }
        map->shouldStop = true;
        t.join();
    }

    void stop() {}

    void init() {
        this->get("Measures")->init();
        this->get("Player")->init();
        this->get("Camera")->init();
        this->get("Map")->init();
        this->get("TaskManager")->init();
        this->get("Chat")->init();
        this->get("Inventory")->init();
        this->get("RenderWindow")->init();

        socket->on("hello",
            [&](JSON data) {
                player->id = data.asInt();
                player->walk(player->position);
            }
        );

        socket->on("positions",
            [&](JSON json) {
                auto _positions = json.children;
                for (auto data : _positions) {
                    int otherid = data["id"].asInt();
                    int x = data["x"].asDouble();
                    int y = data["y"].asDouble();
                    if (otherid == player->id) {
                        player->position.x = x;
                        player->position.y = y;
                    }
                    else {
                        positions[otherid] = VTile(x, y);
                    }
                }
            }
        );
        this->get("Socket")->init();
    }
};
