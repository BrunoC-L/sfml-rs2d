#pragma once
#include <SFML/Graphics.hpp>
#include "units.h"
#include "service.h"
#include "abstractServiceProvider.h"
using namespace std;

class BottomBanner : public Service {
public:
    BottomBanner(AbstractServiceProvider* provider);
    void draw() const;
private:
    BottomBanner();
    sf::RectangleShape banner;
};