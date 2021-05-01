#include "rightBanner.h"
#include "abstractInventory.h"
#include "abstractRenderWindow.h"
#include "abstractPlayer.h"

RightBanner::RightBanner(ServiceProvider* provider, AbstractRenderWindow* window): Service(provider), minimap(provider, window), window(window) {
    acquire();
    banner = sf::RectangleShape(sf::Vector2f(measures->rightBannerWidth, window->getSize().y));
}

void RightBanner::draw() {
    sf::Transform transform;
    transform.scale(sf::Vector2f(1 / measures->stretch.x, 1 / measures->stretch.y));
    transform.translate(sf::Vector2f(measures->getRightBannerStartingX(), 0));
    transform.translate(window->getSize().x - AbstractMeasures::startingScreenSize().x, 0);
    window->draw(&banner, transform);
    minimap.draw();
}

bool RightBanner::mouseIsInRect(MouseEvent& ev) {
    int pxFromRightBorder = window->getSize().x - ev.pos.x;
    return pxFromRightBorder <= AbstractMeasures::rightBannerWidth;
}

void RightBanner::click(MouseEvent& ev) {
    constexpr auto offsetx = AbstractMeasures::minimapRadius + (AbstractMeasures::rightBannerWidth - 2 * AbstractMeasures::minimapRadius) / 2;
    VPixel middleOfMinimap(window->getSize().x - offsetx, AbstractMeasures::minimapRadius);
    VPixel vpxFromMiddleOfMinimap = ev.pos - middleOfMinimap;
    constexpr auto r2 = AbstractMeasures::minimapRadius * AbstractMeasures::minimapRadius;
    auto d2 = vpxFromMiddleOfMinimap.x * vpxFromMiddleOfMinimap.x + vpxFromMiddleOfMinimap.y * vpxFromMiddleOfMinimap.y;
    bool clickIsOnMinimap = r2 >= d2;
    if (clickIsOnMinimap) {
        const float radius = pow(d2, 0.5f);
        const float angle = (vpxFromMiddleOfMinimap.x == 0 ? 
            (vpxFromMiddleOfMinimap.y > 0 ? 90 : -90) : 
            (vpxFromMiddleOfMinimap.x > 0 ? 0 : 3.1415926536f) + 
            atan(vpxFromMiddleOfMinimap.y / vpxFromMiddleOfMinimap.x)) - 
            measures->angle / 180 * 3.1415926536f;
        VPixel rotatedDelta = VPixel(cos(angle), sin(angle)) * radius;
        vpxFromMiddleOfMinimap = rotatedDelta;

        VTile position = player->getPosition() + vpxFromMiddleOfMinimap / AbstractMeasures::pixelsPerTileOnMap;
        WalkClickEvent(position).emit();
    }
}

void RightBanner::update() {
    minimap.update();
}
