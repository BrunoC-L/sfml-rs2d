#pragma once
#include <SFML/Graphics.hpp>
#include "units.h"
#include "abstractMeasures.h"
#include "abstractPlayer.h"
#include "mouseEvent.h"
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
    bool mouseIsInRect(MouseEvent& ev);
    void click(MouseEvent& ev);
    void update();
};
