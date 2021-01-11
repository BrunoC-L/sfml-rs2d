#pragma once
#include <SFML/Graphics.hpp>
#include "../../common/units.h"
#include "service.h"
#include "../../common/abstractServiceProvider.h"
#include "abstractRenderWindow.h"
using namespace std;

class BottomBanner : public Service {
public:
    BottomBanner(AbstractServiceProvider* provider, AbstractRenderWindow* window);
    void draw() const;
private:
    AbstractRenderWindow* window;
    BottomBanner();
    sf::RectangleShape banner;
};