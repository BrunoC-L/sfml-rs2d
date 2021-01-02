#pragma once
#include <thread>

#include "json.h"

#include "units.h"
#include "pathfinder.h"
#include "taskManager.h"
#include "movingPredicate.h"
#include "gameTick.h"
#include "mouseWheelEvent.h"
#include "event.h"
#include "VPixelToVTileConverter.h"
#include "mouseLeftClickEvent.h"
#include "mouseRightClickEvent.h"
#include "mouseMoveEvent.h"
#include "abstractServiceProvider.h"
#include "rightBanner.h"
#include "bottomBanner.h"
#include "rightClickInterface.h"
#include <SFML/Network.hpp>

#define TEMPLATES \
<\
 typename Measures\
,typename RenderWindow\
,typename TaskManager\
,typename Player\
,typename Camera\
,typename Map\
,typename Chat\
,typename Inventory\
>\

template TEMPLATES
class App : public AbstractServiceProvider {
    Measures* measures;
    Map* map;
    Player* player;
    TaskManager* taskManager;
    Camera* camera;
    Chat* chat;
    Inventory* inventory;
    RenderWindow* renderWindow;
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
    }

	void start() {
        int id = 0;
        std::vector<VTile> positions(10);
        sf::TcpSocket socket;
        std::thread tt(
            [&]() {
                const std::string ip = "127.0.0.1";
                const unsigned port = 38838;
                sf::Socket::Status status = socket.connect(ip, 38838);
                if (status != sf::Socket::Done)
                    throw std::exception(std::string("Could not connect to " + ip + ':' + std::to_string(port) + '\n').c_str());
                else
                    cout << "Connected to " + ip + ':' + std::to_string(port) + '\n';

                std::string buffer = "";
                std::string messageEnd = "|END|";

                while (true) {
                    char data[100] = { 0 };
                    std::size_t received;
                    if (socket.receive(data, 100, received) != sf::Socket::Done)
                        continue;

                    buffer += std::string(data).substr(0, received);
                    int index = 0;

                    while ((index = buffer.find(messageEnd)) != -1) {
                        std::string str(buffer.substr(0, index));
                        JSON json(str);
                        if (json["type"].asString() == "position") {
                            auto data = json["data"];
                            int otherid = data["id"].asInt();
                            int x = data["x"].asDouble();
                            int y = data["y"].asDouble();
                            positions[otherid] = VTile(x, y);
                        } else if (json["type"].asString() == "hello") {
                            id = json["data"].asInt();
                        }
                        buffer = buffer.substr(index + messageEnd.length());
                    }
                }
            }
        );
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
            JSON positionUpdate;
            positionUpdate["type"] = "position";
            JSON data;
            data["id"] = to_string(id);
            data["x"] = to_string(player->position.x);
            data["y"] = to_string(player->position.y);
            positionUpdate["data"] = data;
            std::string msg = positionUpdate.asString() + "|END|";
            socket.send(msg.c_str(), msg.length());
            auto dt = clock.getElapsedTime().asMilliseconds();
            if (dt > 1100.f / 60)
                cout << "frame took " << dt << " ms" << endl;
            clock.restart();
            ++frame;
            tickmod = frame % unsigned(Measures::framesPerTick);
            isGameTick = !tickmod;
            if (isGameTick) {
                GameTick::tick();
                taskManager->executeAndRemove();
                player->onGameTick();
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
                if (i == id)
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
    }
};
