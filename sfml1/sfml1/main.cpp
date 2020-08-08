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

int main() {
    Textures textures;
    Measures measures;
    Vector2f startingScreenSize(measures.startingScreenSize.x, measures.startingScreenSize.y);
    RenderWindow window(VideoMode(startingScreenSize.x, startingScreenSize.y), "RS2D");
    measures.setGetWindowSize([&]() { return VPixel(window.getSize().x, window.getSize().y); });
    Player player(window, measures, VTile(18 * Measures::TilesPerChunk + 20, 13 * Measures::TilesPerChunk + 37, 0)); // lumbridge
    VTile  cameraPos = player.position;
    Minimap minimap(window, cameraPos, measures);
    Map map(window, cameraPos, measures, textures, 1);
    RightBanner rightBanner(window, measures);
    BottomBanner bottomBanner(window, measures);
    std::thread t(&Map::doUpdates, &map);
    vector<VTile> path = {};
    auto canMoveToLambda = [&](VTile a, VTile b) {
        VChunk ca = VChunk(int(a.x / Measures::TilesPerChunk), int(a.y / Measures::TilesPerChunk));
        VChunk cb = VChunk(int(b.x / Measures::TilesPerChunk), int(b.y / Measures::TilesPerChunk));
        VChunk da = ca - map.centerChunk + VChunk(map.loaded.size() / 2, map.loaded.size() / 2);
        VChunk db = cb - map.centerChunk + VChunk(map.loaded.size() / 2, map.loaded.size() / 2);
        if (da.x < 0 || db.x < 0 || da.x >= map.loaded.size() || db.x >= map.loaded.size() || da.y < 0 || db.y < 0 || da.y >= map.loaded.size() || db.y >= map.loaded.size())
            return false;
        Chunk* tileAChunk = map.loaded[da.x][da.y];
        Chunk* tileBChunk = map.loaded[db.x][db.y];
        Tile* ta = tileAChunk->tiles[int(a.x - ca.x * Measures::TilesPerChunk)][int(a.y - ca.y * Measures::TilesPerChunk)];
        Tile* tb = tileBChunk->tiles[int(b.x - cb.x * Measures::TilesPerChunk)][int(b.y - cb.y * Measures::TilesPerChunk)];
        return ta->canMoveFrom(*tb);
    };
    
    window.setFramerateLimit(60);
    unsigned tick = 0;
    unsigned tickmod = 0;
    unsigned gameTick = 0;
    while (window.isOpen()) {
        ++tick;
        gameTick = tick / 36;
        tickmod = tick % 36;
        if (!(tick % 36)) {
            player.onGameTick(path);
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
                VTile tileClicked = cameraPos + VTile(deltaTilesFloat.x * signs.x, deltaTilesFloat.y * signs.y) + VTile(0.5, 0.5);
                // if click is in loaded chunk ...
                tileClicked = VTile(int(tileClicked.x), int(tileClicked.y));
                if (!event.mouseButton.button)
                    path = Pathfinder::pathfind(player.positionNextTick, tileClicked, canMoveToLambda);
                else
                    path = { tileClicked };
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
        cameraPos = player.position;

        window.clear();

        map.draw();
        player.draw(cameraPos);

        bottomBanner.draw();
        rightBanner.draw();
        minimap.draw();

        window.display();
    }
    map.shouldStop = true;
    t.join();
}
