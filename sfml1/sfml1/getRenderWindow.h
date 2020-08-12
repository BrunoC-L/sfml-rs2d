#pragma once
#include "measures.h"
#include <SFML/Graphics.hpp>

using namespace sf;

class RenderWindowSingleton {
public:
    static RenderWindow& getInstance() {
        Vector2f startingScreenSize(Measures::getInstance().startingScreenSize.x, Measures::getInstance().startingScreenSize.y);
        static RenderWindow window(VideoMode(startingScreenSize.x, startingScreenSize.y), "RS2D");
        return window;
    }
};