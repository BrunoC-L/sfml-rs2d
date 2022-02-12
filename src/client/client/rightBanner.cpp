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

void RightBanner::update() {
    minimap.update();
}
