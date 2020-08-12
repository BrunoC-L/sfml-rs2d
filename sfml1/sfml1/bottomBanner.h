#pragma once
#include <SFML/Graphics.hpp>
#include "units.h"
#include "measures.h"
#include "getRenderWindow.h"
using namespace std;
using namespace sf;

class BottomBanner {
public:
    BottomBanner();
    void draw() const;
    void update();
private:
    RectangleShape banner;
};