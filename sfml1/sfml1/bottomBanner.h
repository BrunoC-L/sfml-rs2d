#pragma once
#include <SFML/Graphics.hpp>
#include "units.h"
#include "measures.h"
using namespace std;
using namespace sf;

class BottomBanner {
public:
    BottomBanner(RenderWindow& w, Measures& measures);
    void draw() const;
    void update();
private:
    Measures& measures;
    RenderWindow& w;
    RectangleShape banner;
};