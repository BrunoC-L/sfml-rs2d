#pragma once
#include <SFML/Graphics.hpp>
#include "units.h"
#include "abstractMeasures.h"
#include "abstractPlayer.h"
#include "minimap.h"
#include "item.h"
#include "service.h"
#include "chunk.h"
#include "button.h"
#include "textures.h"
#include "innerRightBanner.h"

class RightBanner : public Service {
private:
    AbstractRenderWindow* window;
    Minimap minimap;
    sf::RectangleShape banner;

    std::shared_ptr<InnerRightBanner> innerBanner;

    const int nButtonsRow = 7;
    const VPixel buttonSize = VPixel(35, 38);
    const VPixel innerTabSize = VPixel(nButtonsRow * buttonSize.x, 300);

    std::shared_ptr<Button> combatTabButton;
    std::shared_ptr<Button> skillsTabButton;
    std::shared_ptr<Button> questsTabButton;
    std::shared_ptr<Button> inventoryTabButton;
    std::shared_ptr<Button> equipmentTabButton;
    std::shared_ptr<Button> prayersTabButton;
    std::shared_ptr<Button> magicTabButton;

    std::vector<std::shared_ptr<Button>*> topRowTabButtons = {
        &combatTabButton,
        &skillsTabButton,
        &questsTabButton,
        &inventoryTabButton,
        &equipmentTabButton,
        &prayersTabButton,
        &magicTabButton,
    };

    std::vector<sf::Texture*> topRowTabButtonTextures = {
        &combatTabButtonTexture,
        &skillsTabButtonTexture,
        &questsTabButtonTexture,
        &inventoryTabButtonTexture,
        &equipmentTabButtonTexture,
        &prayersTabButtonTexture,
        &magicTabButtonTexture,
    };

    std::shared_ptr<Button> clanTabButton;
    std::shared_ptr<Button> friendsTabButton;
    std::shared_ptr<Button> ignoreTabButton;
    std::shared_ptr<Button> logoutTabButton;
    std::shared_ptr<Button> settingsTabButton;
    std::shared_ptr<Button> emotesTabButton;
    std::shared_ptr<Button> musicTabButton;

    std::vector<std::shared_ptr<Button>*> bottomRowTabButtons = {
        &clanTabButton,
        &friendsTabButton,
        &ignoreTabButton,
        &logoutTabButton,
        &settingsTabButton,
        &emotesTabButton,
        &musicTabButton,
    };

    std::vector<sf::Texture*> bottomRowTabButtonTextures = {
        &clanTabButtonTexture,
        &friendsTabButtonTexture,
        &ignoreTabButtonTexture,
        &logoutTabButtonTexture,
        &settingsTabButtonTexture,
        &emotesTabButtonTexture,
        &musicTabButtonTexture,
    };

public:
    RightBanner(ServiceProvider* provider, AbstractRenderWindow* window);
    void draw();
    template <class MouseEvent>
    bool mouseIsInRect(const MouseEvent& ev) {
        int pxFromRightBorder = window->getSize().x - ev.pos.x;
        return pxFromRightBorder <= AbstractMeasures::rightBannerWidth;
    }
    void click(const MouseLeftClickEvent::Data& ev) {
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

            VTile position = player->getPosition() + vpxFromMiddleOfMinimap / PIXELS_PER_TILE_ON_MAP;
            EVENT(WalkClickEvent, position).emit();
        }
    }
    void update();
};
