#pragma once
#include <SFML/Graphics.hpp>
#include "units.h"
#include "abstractServices.h"
#include "serviceProvider.h"
#include "rightClickTileEvent.h"
#include "service.h"
#include "abstractRenderWindow.h"
#include "mouseLeftClickEvent.h"

class RightClickInterface : public Service {
private:
    AbstractRenderWindow* window;
    std::vector<ObjectInteractions> interactions;
    sf::Text text;
    sf::Font font;
    sf::RectangleShape rect;
    RightClickTileEvent::Observer rightClickTileObserver;
    Tile* tile;
public:
    RightClickInterface(ServiceProvider* provider, AbstractRenderWindow* window);
    void draw();
    bool active;
    void setPosition(VPixel pos);
    template <class MouseEvent>
    bool mouseIsInRect(const MouseEvent& ev) {
        auto delta = getDeltaClick(ev);
        return delta.x < rect.getSize().x&& delta.y < rect.getSize().y&& delta.x >= 0 && delta.y >= 0;
    }
    template <class MouseEvent>
    VPixel getDeltaClick(const MouseEvent& ev) {
        auto rectPos = rect.getPosition();
        return VPixel(ev.pos.x - rectPos.x, ev.pos.y - rectPos.y);
    }
    void click(const MouseLeftClickEvent::Data& ev) {
        active = false;
        if (!interactions.size())
            return;
        auto dy = getDeltaClick(ev).y - 16;
        int interactionIndex = dy / 16;
        if (dy < 0 || interactionIndex >= interactions.size())
            return;
        auto interaction = interactions[interactionIndex];
        JSON data;
        data["x"] = std::to_string(interaction.tile.x);
        data["y"] = std::to_string(interaction.tile.y);
        data["object"] = "{}";
        data["object"]["state"] = std::to_string(interaction.objectState);
        data["object"]["interaction"] = interaction.interactions[0];
        data["object"]["objectName"] = interaction.objectName;
        socket->send("interact", data);
    }
    void addText(std::string text);
    void resetText();
    void addInteractions(ObjectInteractions interactions);
};
