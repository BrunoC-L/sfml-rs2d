#pragma once
#include <SFML/Graphics.hpp>
#include "units.h"
#include "measures.h"
using namespace std;
using namespace sf;

class RightBanner {
public:
    RightBanner(RenderWindow& w, Measures& measures);
    void draw() const;
    void update();
private:
    Measures& measures;
    RenderWindow& w;
    RectangleShape banner;
};