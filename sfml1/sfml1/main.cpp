#include <thread>

#include "map.h"
#include "measures.h"
#include "units.h"
#include "rightBanner.h"
#include "bottomBanner.h"
#include "player.h"
#include "pathfinder.h"
#include "taskManager.h"
#include "getRenderWindow.h"
#include "camera.h"
#include "movingPredicate.h"
#include "gameTick.h"
#include "rightClickInterface.h"
#include "mouseEvent.h"

int main() {
    Measures& measures = Measures::getInstance();
    RenderWindow& window = RenderWindowSingleton::getInstance();
    TaskManager& taskManager = TaskManager::getInstance();
    measures.setGetWindowSize([&]() { return VPixel(window.getSize().x, window.getSize().y); });
    Player& player = Player::getInstance();
    VTile lumbridge(18 * Measures::TilesPerChunk + 20, 13 * Measures::TilesPerChunk + 37, 0);
    player.position = lumbridge;
    Camera& camera = Camera::getInstance();
    camera.setPosition(&player.position);
    Map& map = Map::getInstance();
    map.chunkRadius = 1;
    map.load();
    RightBanner& rightBanner = RightBanner::getInstance();
    BottomBanner& bottomBanner = BottomBanner::getInstance();
    RightClickInterface& rightClickInterface = RightClickInterface::getInstance();

    std::thread t(&Map::doUpdates, &map);

    VPixelToVTileConverter& converter = VPixelToVTileConverter::getInstance();

    converter.getPositionInGame(VPixel());
    
    window.setFramerateLimit(60);
    unsigned frame = 0;
    unsigned tickmod = 0;
    bool isGameTick = false;
    Clock clock;

    while (window.isOpen()) {
        auto dt = clock.getElapsedTime().asMilliseconds();
        if (dt > 1100.f/60)
            cout << "frame took " << dt << " ms" << endl;
        clock.restart();
        ++frame;
        tickmod = frame % unsigned(Measures::framesPerTick);
        isGameTick = !tickmod;
        if (isGameTick) {
            GameTick::tick();
            taskManager.executeAndRemove();
            player.onGameTick();
            map.shouldUpdate = true;
        }

        bool clickedOnRightClickInterface = false;
        bool clickedOnRightBanner = false;
        bool clickedOnBottomBanner = false;
        bool clickedOnWorld = true;

        shared_ptr<MouseEvent> mouseEvent = nullptr;
        Event event;

        while (window.pollEvent(event))
            if (event.type == Event::Closed)
                window.close();
            else if (event.type == Event::KeyPressed)
                switch (event.text.unicode) {
                    case 71:
                        measures.angle -= 5;
                        break;
                    case 72:
                        measures.angle += 5;
                        break;
                }
            else if (event.type == Event::MouseButtonPressed) {
                switch (event.mouseButton.button) {
                    default:
                    case Left:
                        mouseEvent = make_shared<MouseLeftClickEvent>(event);
                        clickedOnRightClickInterface = rightClickInterface.active && rightClickInterface.mouseIsInRect(mouseEvent);
                        if (clickedOnRightClickInterface) {
                            clickedOnWorld = false;
                            rightClickInterface.click(mouseEvent);
                        }
                        clickedOnRightBanner = rightBanner.mouseIsInRect(mouseEvent);
                        if (clickedOnRightBanner) {
                            clickedOnWorld = false;
                            rightBanner.click(mouseEvent);
                        }
                        break;
                    case Right:
                        mouseEvent = make_shared<MouseRightClickEvent>(event);
                        if (!rightClickInterface.mouseIsInRect(mouseEvent) || !rightClickInterface.active)
                            rightClickInterface.setPosition(VPixel(event.mouseButton.x, event.mouseButton.y));
                        break;
                    case Middle:
                        mouseEvent = make_shared<MouseMiddleClickEvent>(event);
                        player.clearActionIfNotBusy();
                        break;
                }
                if (clickedOnWorld) {
                    VTile tileClicked = converter.getPositionInGame(mouseEvent->position);
                    Tile* t = map.getTileFromVTile(tileClicked);
                    if (t)
                        mouseEvent->accept(t);
                }
            }
            else if (event.type == Event::Resized)
                measures.update();
            else if (event.type == Event::MouseWheelMoved)
                measures.zoom = measures.zoom * (1 + (0.3 + event.mouseWheel.delta) * 0.1f);
            else if (event.type == Event::MouseMoved) {
                mouseEvent = make_shared<MouseMoveEvent>(event);
                // add top left indicator of what your mouse is over + left click option + options.length
                if (!rightClickInterface.mouseIsInRect(mouseEvent))
                    rightClickInterface.active = false;
            }

        rightBanner.update();
        player.update(tickmod);
        window.clear();

        map.draw();
        player.draw(camera.getPosition());

        bottomBanner.draw();
        rightBanner.draw();
        rightClickInterface.draw();

        window.display();
    }
    map.shouldStop = true;
    t.join();
}
