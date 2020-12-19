#include "rightBanner.h"

RightBanner::RightBanner() {
    Measures& measures = Measures::getInstance();
    banner = RectangleShape(Vector2f(measures.rightBannerWidth, measures.getWindowSize().y));
    RectangleShape defaultShape(Vector2f(Measures::itemsSizeInInventory, Measures::itemsSizeInInventory));
    Texture* texture = new Texture();
    texture->loadFromFile("../../assets/textures/items.png");
    defaultShape.setTexture(texture);
    Vector2i pos(0, 0);
    Vector2i size(Measures::itemsSizePx, Measures::itemsSizePx);
    defaultShape.setTextureRect(IntRect(pos, size));
    inventoryItems = vector<pair<Item, RectangleShape>>(28, make_pair(Item(), RectangleShape(defaultShape)));
}

void RightBanner::draw() {
    Measures& measures = Measures::getInstance();
    RenderWindow& w = RenderWindowSingleton::getInstance();
    Player& player = Player::getInstance();
    Transform transform;
    transform.scale(Vector2f(1 / measures.stretch.x, 1 / measures.stretch.y));
    transform.translate(Vector2f(measures.getRightBannerStartingX(), 0));
    transform.translate(measures.getWindowSize().x - measures.startingScreenSize.x, 0);
    w.draw(banner, transform);
    minimap.draw();
    transform.translate(Vector2f((Measures::rightBannerWidth - Measures::itemsSizeInInventory * 4) / 2, 499 + measures.getWindowSize().y - measures.startingScreenSize.y));

    auto items = player.inventory.getItems();
    for (unsigned i = 0; i < items.size(); ++i) {
        Transform newTransform(transform);
        auto x = i % 4;
        auto y = i / 4;
        newTransform.translate(Vector2f(Measures::itemsSizeInInventory * x, Measures::itemsSizeInInventory * y));
        if (!items[i].isSameTypeAndQty(inventoryItems[i].first)) {
            inventoryItems[i].first = items[i];
            auto px = items[i].textureIndex % int(16384 / Measures::itemsSizePx) * Measures::itemsSizePx;
            auto py = items[i].textureIndex / int(16384 / Measures::itemsSizePx) * Measures::itemsSizePx;
            Vector2i pos(px, py);
            Vector2i size(Measures::itemsSizePx, Measures::itemsSizePx);
            // if item is stackable get the texture with the correct amount
            inventoryItems[i].second.setTextureRect(IntRect(pos, size));
        }
        if (inventoryItems[i].first.id)
            w.draw(inventoryItems[i].second, newTransform);
    }
}

bool RightBanner::mouseIsInRect(MouseEvent* ev) {
    int pxFromRightBorder = Measures::getInstance().getWindowSize().x - ev->pos.x;
    return pxFromRightBorder <= Measures::rightBannerWidth;
}

void RightBanner::click(MouseEvent* ev) {
    Measures& measures = Measures::getInstance();
    Player& player = Player::getInstance();
    constexpr auto offsetx = Measures::minimapRadius + (Measures::rightBannerWidth - 2 * Measures::minimapRadius) / 2;
    VPixel middleOfMinimap(measures.getWindowSize().x - offsetx, Measures::minimapRadius);
    VPixel vpxFromMiddleOfMinimap = ev->pos - middleOfMinimap;
    constexpr auto r2 = Measures::minimapRadius * Measures::minimapRadius;
    auto d2 = vpxFromMiddleOfMinimap.x * vpxFromMiddleOfMinimap.x + vpxFromMiddleOfMinimap.y * vpxFromMiddleOfMinimap.y;
    bool clickIsOnMinimap = r2 >= d2;
    if (clickIsOnMinimap) {
        const float radius = pow(d2, 0.5f);
        const float angle = (vpxFromMiddleOfMinimap.x == 0 ? 
            (vpxFromMiddleOfMinimap.y > 0 ? 90 : -90) : 
            (vpxFromMiddleOfMinimap.x > 0 ? 0 : 3.1415926536f) + 
            atan(vpxFromMiddleOfMinimap.y / vpxFromMiddleOfMinimap.x)) - 
            measures.angle / 180 * 3.1415926536f;
        VPixel rotatedDelta = VPixel(cos(angle), sin(angle)) * radius;
        vpxFromMiddleOfMinimap = rotatedDelta;
        int px = player.position.x + vpxFromMiddleOfMinimap.x / Measures::pixelsPerTileOnMap;
        int py = player.position.y + vpxFromMiddleOfMinimap.y / Measures::pixelsPerTileOnMap;
        VTile position(px, py);
        player.path = Pathfinder::pathfind(player.positionNextTick, { position }, false);
    }
    bool clickIsOnInnerInterfaceOptions = false;
    bool clickIsInInnerInterface = false;
}

void RightBanner::update() {
    minimap.update();
}