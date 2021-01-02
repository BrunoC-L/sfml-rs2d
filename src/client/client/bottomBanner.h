#pragma once
#include <SFML/Graphics.hpp>
#include "units.h"
#include "abstractServices.h"
#include "abstractServiceProvider.h"
using namespace std;

class BottomBanner {
public:
    SERVICE_MEMBERS;
    BottomBanner(AbstractServiceProvider* provider);
    void draw() const;
private:
    BottomBanner();
    sf::RectangleShape banner;
};