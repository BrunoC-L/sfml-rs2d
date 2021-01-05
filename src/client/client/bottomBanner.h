#pragma once
#include <SFML/Graphics.hpp>
#include "../../common/units.h"
#include "service.h"
#include "../../common/abstractServiceProvider.h"
using namespace std;

class BottomBanner : public Service {
public:
    BottomBanner(AbstractServiceProvider* provider);
    void draw() const;
private:
    BottomBanner();
    sf::RectangleShape banner;
};