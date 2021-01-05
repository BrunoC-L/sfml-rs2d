#include "rightBanner.h"
#include "abstractInventory.h"
#include "abstractRenderWindow.h"
#include "abstractPlayer.h"

RightBanner::RightBanner(AbstractServiceProvider* provider): Service(provider), minimap(provider) {
    acquire();
    banner = sf::RectangleShape(sf::Vector2f(measures->rightBannerWidth, measures->getWindowSize().y));
    sf::RectangleShape defaultShape(sf::Vector2f(AbstractMeasures::itemsSizeInInventory, AbstractMeasures::itemsSizeInInventory));
    sf::Texture* texture = new sf::Texture();
    texture->loadFromFile("../../../assets/textures/items.png");
    defaultShape.setTexture(texture);
    sf::Vector2i pos(0, 0);
    sf::Vector2i size(AbstractMeasures::itemsSizePx, AbstractMeasures::itemsSizePx);
    defaultShape.setTextureRect(sf::IntRect(pos, size));
    inventoryItems = vector<pair<Item, sf::RectangleShape>>(28, make_pair(Item(), sf::RectangleShape(defaultShape)));
}

void RightBanner::draw() {
    sf::Transform transform;
    transform.scale(sf::Vector2f(1 / measures->stretch.x, 1 / measures->stretch.y));
    transform.translate(sf::Vector2f(measures->getRightBannerStartingX(), 0));
    transform.translate(measures->getWindowSize().x - AbstractMeasures::startingScreenSize().x, 0);
    renderWindow->draw(&banner, transform);
    minimap.draw();
    transform.translate(
        (AbstractMeasures::rightBannerWidth - AbstractMeasures::itemsSizeInInventory * 4) / 2,
        499 + measures->getWindowSize().y - AbstractMeasures::startingScreenSize().y
    );
    auto items = inventory->getItems();
    for (unsigned i = 0; i < items.size(); ++i) {
        sf::Transform newTransform(transform);
        auto x = i % 4;
        auto y = i / 4;
        newTransform.translate(sf::Vector2f(AbstractMeasures::itemsSizeInInventory * x, AbstractMeasures::itemsSizeInInventory * y));
        if (!items[i].isSameTypeAndQty(inventoryItems[i].first)) {
            inventoryItems[i].first = items[i];
            auto px = items[i].textureIndex % int(16384 / AbstractMeasures::itemsSizePx) * AbstractMeasures::itemsSizePx;
            auto py = items[i].textureIndex / int(16384 / AbstractMeasures::itemsSizePx) * AbstractMeasures::itemsSizePx;
            sf::Vector2i pos(px, py);
            sf::Vector2i size(AbstractMeasures::itemsSizePx, AbstractMeasures::itemsSizePx);
            // if item is stackable get the texture with the correct amount
            inventoryItems[i].second.setTextureRect(sf::IntRect(pos, size));
        }
        if (inventoryItems[i].first.id)
            renderWindow->draw(&inventoryItems[i].second, newTransform);
    }
}

bool RightBanner::mouseIsInRect(MouseEvent* ev) {
    int pxFromRightBorder = measures->getWindowSize().x - ev->pos.x;
    return pxFromRightBorder <= AbstractMeasures::rightBannerWidth;
}

void RightBanner::click(MouseEvent* ev) {
    constexpr auto offsetx = AbstractMeasures::minimapRadius + (AbstractMeasures::rightBannerWidth - 2 * AbstractMeasures::minimapRadius) / 2;
    VPixel middleOfMinimap(measures->getWindowSize().x - offsetx, AbstractMeasures::minimapRadius);
    VPixel vpxFromMiddleOfMinimap = ev->pos - middleOfMinimap;
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

        int px = player->position.x + vpxFromMiddleOfMinimap.x / AbstractMeasures::pixelsPerTileOnMap;
        int py = player->position.y + vpxFromMiddleOfMinimap.y / AbstractMeasures::pixelsPerTileOnMap;
        VTile position(px, py);
        // player->path = Pathfinder::pathfind(player->positionNextTick, { position }, false, map);
    }
    bool clickIsOnInnerInterfaceOptions = false;
    bool clickIsInInnerInterface = false;
}

void RightBanner::update() {
    minimap.update();
}