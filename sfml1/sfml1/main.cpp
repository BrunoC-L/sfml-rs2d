#include <iostream>
#include "minimap.h"
#include "map.h"
#include "measures.h"
#include "debug.h"
#include "units.h"
#include <thread>

int main() {
    Measures measures;
    Vector2f x(measures.startingScreenSize.x, measures.startingScreenSize.y);
    RenderWindow window(VideoMode(x.x, x.y), "RS2D");
    measures.setGetWindowSize([&]() { return VPixel(window.getSize().x, window.getSize().y); });
    VTile playerPos(18 * 64 + 20, 13 * 64 + 37, 0); // lumbridge 
    Minimap minimap(window, playerPos, measures);
    Map map(window, playerPos, measures, 1);
    std::thread t(&Map::doUpdates, &map);

    RectangleShape right, bottom;
    
    window.setFramerateLimit(60);
    while (window.isOpen()) {
        right.setPosition(measures.getWindowSize().x - measures.rightBannerWidth, 0);
        right.setSize(Vector2f(measures.rightBannerWidth, measures.getWindowSize().y));

        bottom.setPosition(0, measures.getWindowSize().y - measures.bottomBannerHeight);
        bottom.setSize(Vector2f(measures.getWindowSize().x, measures.bottomBannerHeight));

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
                playerPos += VTile(
                    event.mouseButton.x >= measures.getWindowSize().x * 2 / 3 ? 1 : event.mouseButton.x <= measures.getWindowSize().x / 3 ? -1 : 0,
                    event.mouseButton.y >= measures.getWindowSize().y * 2 / 3 ? 1 : event.mouseButton.y <= measures.getWindowSize().y / 3 ? -1 : 0,
                    0
                );
                print(playerPos);
                print(playerPos - VTile(int(playerPos.x / 64), int(playerPos.y) / 64) * 64);
            }
            else if (event.type == Event::Resized)
                print((Vector2i)window.getSize());
            else if (event.type == Event::MouseWheelMoved)
                measures.zoom = Vector2f(measures.zoom.x * (1 + (event.mouseWheel.delta + 0.3) * 0.1f), measures.zoom.y * (1 + (event.mouseWheel.delta + 0.3) * 0.1f));

        measures.update();
        minimap.update();
        window.clear();
        map.draw();
        window.draw(right, measures.getRightBannerRS());
        window.draw(bottom, measures.getBottomBannerRS());
        minimap.draw();
        window.display();
    }
    map.shouldStop = true;
    t.join();
}
