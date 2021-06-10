#include "rightBanner.h"
#include "abstractInventory.h"
#include "abstractRenderWindow.h"
#include "abstractPlayer.h"
#include "logoutInnerBanner.h"

RightBanner::RightBanner(ServiceProvider* provider, AbstractRenderWindow* window): Service(provider), minimap(provider, window), window(window) {
    acquire();
    banner = sf::RectangleShape(sf::Vector2f(measures->rightBannerWidth, window->getSize().y));

    VPixel topButtonPos = VPixel( - innerTabSize.x, - 2 * buttonSize.y - innerTabSize.y);
    VPixel bottomButtonPos = VPixel(- innerTabSize.x, - buttonSize.y);
    for (int i = 0; i < nButtonsRow; ++i) {
        (*topRowTabButtons[i]) = std::make_shared<Button>(std::make_shared<AnchoredOffsetTransform>(BottomRightAnchorTransform, topButtonPos), buttonSize);
        (*topRowTabButtons[i])->texture(*topRowTabButtonTextures[i]);
        (*bottomRowTabButtons[i]) = std::make_shared<Button>(std::make_shared<AnchoredOffsetTransform>(BottomRightAnchorTransform, bottomButtonPos), buttonSize);
        (*bottomRowTabButtons[i])->texture(*bottomRowTabButtonTextures[i]);
        topButtonPos += VPixel(buttonSize.x, 0);
        bottomButtonPos += VPixel(buttonSize.x, 0);
    }

    logoutTabButton->onClick([&]() {
        innerBanner = std::make_shared<LogoutInnerBanner>(
            this->provider,
            std::make_shared<AnchoredOffsetTransform>(BottomRightAnchorTransform, VPixel(-innerTabSize.x, -innerTabSize.y - buttonSize.y))
        );
    });
}

void RightBanner::draw() {
    sf::Transform transform;
    transform.scale(sf::Vector2f(1 / measures->stretch.x, 1 / measures->stretch.y));
    transform.translate(sf::Vector2f(measures->getRightBannerStartingX(), 0));
    auto sizeChange = window->getSize().x - AbstractMeasures::startingScreenSize().x;
    transform.translate(sizeChange, 0);
    window->draw(&banner, transform);
    minimap.draw();

    for (auto* tabButton : topRowTabButtons)
        (*tabButton)->draw(*window);

    for (auto* tabButton : bottomRowTabButtons)
        (*tabButton)->draw(*window);

    if (innerBanner)
        innerBanner->draw(window);
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
