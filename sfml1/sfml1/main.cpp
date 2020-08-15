#include <iostream>
#include <thread>
#include <math.h>

#include "minimap.h"
#include "map.h"
#include "measures.h"
#include "debug.h"
#include "units.h"
#include "rightBanner.h"
#include "bottomBanner.h"
#include "player.h"
#include "pathfinder.h"
#include "Textures.h"
#include "taskManager.h"
#include "getRenderWindow.h"
#include "camera.h"
#include "movingPredicate.h"
#include "gameTick.h"

int main() {
    Textures& textures = Textures::getInstance();
    Measures& measures = Measures::getInstance();
    RenderWindow& window = RenderWindowSingleton::getInstance();
    TaskManager& taskManager = TaskManager::getInstance();
    measures.setGetWindowSize([&]() { return VPixel(window.getSize().x, window.getSize().y); });
    Player& player = Player::getInstance();
    player.position = VTile(18 * Measures::TilesPerChunk + 20, 13 * Measures::TilesPerChunk + 37, 0); // lumbridge
    Camera& camera = Camera::getInstance();
    camera.setPosition(&player.position);
    Map& map = Map::getInstance();
    map.chunkRadius = 1;
    map.load();
    Minimap minimap;
    RightBanner rightBanner;
    BottomBanner bottomBanner;

    std::thread t(&Map::doUpdates, &map);
    
    window.setFramerateLimit(60);
    unsigned tick = 0;
    unsigned tickmod = 0;
    bool isGameTick = false;

    while (window.isOpen()) {
        ++tick;
        tickmod = tick % 36;
        isGameTick = !tickmod;
        if (isGameTick) {
            GameTick::tick();
            taskManager.executeAndRemove();
            player.onGameTick();
        }
        map.shouldUpdate = true;

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
                VPixel click(event.mouseButton.x, event.mouseButton.y);
                VPixel middle(measures.getInnerWindowSize() / 2);
                VPixel delta = click - middle;
                const float radius = pow(pow(delta.x, 2) + pow(delta.y, 2), 0.5f);
                const float angle = (delta.x == 0 ? (delta.y > 0 ? 90 : -90) : (delta.x > 0 ? 0 : 3.1415926536f) + atan(delta.y / delta.x)) - measures.angle / 180 * 3.1415926536f;
                VPixel rotatedDelta = VPixel(cos(angle), sin(angle)) * radius;
                VTile signs(rotatedDelta.x > 0 ? 1 : -1, rotatedDelta.y > 0 ? 1 : -1);
                rotatedDelta *= VPixel(signs.x, signs.y);
                rotatedDelta /= measures.zoom;
                VTile deltaTilesFloat = VTile(rotatedDelta.x, rotatedDelta.y) / Measures::pixelsPerTile;
                VTile tileClicked = camera.getPosition() + VTile(deltaTilesFloat.x * signs.x, deltaTilesFloat.y * signs.y) + VTile(0.5, 0.5);

                VChunk vc = VChunk(int(tileClicked.x / Measures::TilesPerChunk), int(tileClicked.y / Measures::TilesPerChunk));
                VChunk deltaChunks = vc - map.centerChunk + VChunk(map.loaded.size() / 2, map.loaded.size() / 2);
                Tile* t;

                if (deltaChunks.x >= 0 && deltaChunks.x < map.loaded.size() && deltaChunks.y >= 0 && deltaChunks.y < map.loaded.size()) {
                    Chunk* chunk = map.loaded[deltaChunks.x][deltaChunks.y];
                    t = chunk->tiles[int(tileClicked.x - vc.x * Measures::TilesPerChunk)][int(tileClicked.y - vc.y * Measures::TilesPerChunk)];
                    player.currentAction = t->click(event);
                }

                if (!player.currentAction.first) {
                    tileClicked = VTile(int(tileClicked.x), int(tileClicked.y));
                    if (!event.mouseButton.button)
                        player.path = Pathfinder::pathfind(player.positionNextTick, { tileClicked }, false);
                    else
                        player.path = { tileClicked };
                }
            }
            else if (event.type == Event::Resized)
                measures.update();
            else if (event.type == Event::MouseWheelMoved)
                if (event.mouseWheel.delta > 0)
                    measures.zoom = measures.zoom * (1 + (event.mouseWheel.delta + 0.3) * 0.1f);
                else
                    measures.zoom = measures.zoom / (1 + (-event.mouseWheel.delta + 0.3) * 0.1f);

        rightBanner.update();
        bottomBanner.update();
        minimap.update();
        player.update(tickmod);

        window.clear();

        map.draw();
        player.draw(camera.getPosition());

        bottomBanner.draw();
        rightBanner.draw();
        minimap.draw();

        window.display();
    }
    map.shouldStop = true;
    t.join();
}
