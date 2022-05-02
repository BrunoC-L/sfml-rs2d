#include "bottomBanner.h"
#include "anchorTransform.h"
#include "font.h"

BottomBanner::BottomBanner(ServiceProvider* provider, AbstractRenderWindow* window) : Service(provider), window(window) {
    acquire();
    banner = sf::RectangleShape(sf::Vector2f(window->getSize().x, measures->bottomBannerHeight));
    banner.setPosition(0, measures->getBottomBannerStartingY());
    this->chat = (AbstractChat*)provider->get(SERVICES::CHAT);
}

void BottomBanner::draw() const {
    const auto scale = measures->stretch;
    window->draw(
        &banner,
        sf::RenderStates().transform.scale(sf::Vector2f(1, 1 / scale.y)).translate(0, window->getSize().y - AbstractMeasures::startingScreenSize().y)
    );
    auto transform(BottomLeftAnchorTransform.getTransform());
    int pxFromBottom = 6;
    int offsetCorrection = 3;
    int charSize = 20;
    int offset = charSize - offsetCorrection;
    int pxleft = measures->bottomBannerHeight - offsetCorrection - pxFromBottom - offset;
    int spacingBetweenParts = 3;
    transform.translate(0, -offsetCorrection - pxFromBottom);
    transform.translate(0, -offset);
    sf::Text currentlyTyped(chat->getCurrentMessage(), font);
    currentlyTyped.setFillColor(sf::Color::Black);
    currentlyTyped.setCharacterSize(charSize);
    window->draw(currentlyTyped, transform);
    for (auto it = chat->getMessages().rbegin(); it != chat->getMessages().rend(); ++it) {
        const auto& m = *it;
        int deltaX = 0;
        transform.translate(0, -offset);
        for (auto part : m.parts) {
            sf::Text text(part.first, font);
            switch (part.second) {
            case Color::BLACK:
                text.setFillColor(sf::Color::Black);
                break;
            case Color::BLUE:
                text.setFillColor(sf::Color::Blue);
                break;
            case Color::YELLOW:
                text.setFillColor(sf::Color::Yellow);
                break;
            case Color::RED:
                text.setFillColor(sf::Color::Red);
                break;
            }
            text.setCharacterSize(charSize);
            pxleft -= offset;
            if (pxleft < 0)
                break;
            transform.translate(deltaX, 0);
            window->draw(text, transform);
            transform.translate(-deltaX, 0);
            deltaX += text.getLocalBounds().width + spacingBetweenParts;
        }
    }
}
