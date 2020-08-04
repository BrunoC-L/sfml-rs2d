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

int main() {
    Measures measures;
    Vector2f startingScreenSize(measures.startingScreenSize.x, measures.startingScreenSize.y);
    RenderWindow window(VideoMode(startingScreenSize.x, startingScreenSize.y), "RS2D");
    measures.setGetWindowSize([&]() { return VPixel(window.getSize().x, window.getSize().y); });
    Player player(window, measures, VTile(18 * 64 + 20, 13 * 64 + 37, 0)); // lumbridge
    VTile& playerPos = player.position;
    Minimap minimap(window, playerPos, measures);
    Map map(window, playerPos, measures, 1);
    RightBanner rightBanner(window, measures);
    BottomBanner bottomBanner(window, measures);

    std::thread t(&Map::doUpdates, &map);
    
    window.setFramerateLimit(60);
    while (window.isOpen()) {
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
                const float angle = (delta.x == 0 ? (delta.y > 0 ? 90 : -90) : (delta .x > 0 ? 0 : 3.1415926536f) + atan(delta.y / delta.x)) - measures.angle / 180 * 3.1415926536f;
                VPixel rotatedDelta = VPixel(cos(angle), sin(angle)) * radius;
                VTile signs(rotatedDelta.x > 0 ? 1 : -1, rotatedDelta.y > 0 ? 1 : -1);
                rotatedDelta *= VPixel(signs.x, signs.y);
                rotatedDelta /= measures.zoom;
                VTile deltaTilesFloat = VTile(rotatedDelta.x, rotatedDelta.y) / measures.pixelsPerTile + VTile(0.5, 0.5);
                playerPos += VTile(int(deltaTilesFloat.x * signs.x), int(deltaTilesFloat.y * signs.y));
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
        player.update();

        window.clear();

        map.draw();
        bottomBanner.draw();
        rightBanner.draw();
        minimap.draw();
        player.draw();

        window.display();
    }
    map.shouldStop = true;
    t.join();
}
