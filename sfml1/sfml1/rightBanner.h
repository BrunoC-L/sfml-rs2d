#pragma once
#include <SFML/Graphics.hpp>
#include "units.h"
#include "measures.h"
#include "getRenderWindow.h"
using namespace std;
using namespace sf;

class RightBanner {
public:
    RightBanner();
    void draw() const;
private:
    RectangleShape banner;
};