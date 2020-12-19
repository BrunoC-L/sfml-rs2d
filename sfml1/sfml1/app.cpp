#include "app.h"

// #include <SFML/Network.hpp>
template <
    typename Measures,
    typename RenderWindow,
    typename TaskManager,
    typename Player,
    typename Camera,
    typename Map,
    typename RightBanner,
    typename BottomBanner,
    typename RightClickInterface
>
void App<
    typename Measures,
    typename RenderWindow,
    typename TaskManager,
    typename Player,
    typename Camera,
    typename Map,
    typename RightBanner,
    typename BottomBanner,
    typename RightClickInterface
>::main() {
    //sf::TcpSocket socket;
    //sf::Socket::Status status = socket.connect("127.0.0.1", 5000);
    //if (status != sf::Socket::Done)
    //    cout << "WTF";
    //else
    //    cout << "it works!";
    //char data[100] = { 0 };
    //cout << data;
    //std::size_t received;

    //// TCP socket:
    //if (socket.receive(data, 100, received) != sf::Socket::Done)
    //    cout << "WTF";
    //else
    //    cout << ' ' << data;

    Measures& measures = Measures::getInstance();
    RenderWindow& window = RenderWindowSingleton::getInstance();
    TaskManager& taskManager = TaskManager::getInstance();
    measures.setGetWindowSize([&]() { return VPixel(window.getSize().x, window.getSize().y); });
    Player& player = Player::getInstance();
    VTile lumbridge(18 * Measures::TilesPerChunk + 20, 13 * Measures::TilesPerChunk + 37, 0);
    player.position = lumbridge;
    Camera& camera = Camera::getInstance();
    camera.setPosition(&player.position);
    Map& map = Map::getInstance();
    map.chunkRadius = 1;
    map.load();
    RightBanner& rightBanner = RightBanner::getInstance();
    BottomBanner& bottomBanner = BottomBanner::getInstance();
    RightClickInterface& rightClickInterface = RightClickInterface::getInstance();

    std::thread t(&Map::doUpdates, &map);

    VPixelToVTileConverter& converter = VPixelToVTileConverter::getInstance();

    converter.getPositionInGame(VPixel());

    window.setFramerateLimit(60);
    unsigned frame = 0;
    unsigned tickmod = 0;
    bool isGameTick = false;
    Clock clock;

    bool clickedOnRightClickInterface = false;
    bool clickedOnRightBanner = false;
    bool clickedOnWorld = true;
    // bool clickedOnBottomBanner = false;

    // REMOVE LATER
    auto x = new EventObserver<MouseWheelEvent>([&](MouseWheelEvent* ev) {
        measures.zoom = measures.zoom * (1 + (0.3 + ev->delta) * 0.1f);
        });
    MouseWheelEvent::subscribe(x);

    auto x2 = new EventObserver<MouseLeftClickEvent>([&](MouseLeftClickEvent* ev) {
        clickedOnRightClickInterface = rightClickInterface.active && rightClickInterface.mouseIsInRect(ev);
        if (clickedOnRightClickInterface) {
            clickedOnWorld = false;
            rightClickInterface.click(ev);
        }
        clickedOnRightBanner = rightBanner.mouseIsInRect(ev);
        if (clickedOnRightBanner) {
            clickedOnWorld = false;
            rightBanner.click(ev);
        }
        if (clickedOnWorld) {
            VTile tileClicked = converter.getPositionInGame(ev->pos);
            Tile* t = map.getTileFromVTile(tileClicked);
            if (t)
                t->onLeftClick(*ev);
        }
        });
    MouseLeftClickEvent::subscribe(x2);

    auto x3 = new EventObserver<MouseRightClickEvent>([&](MouseRightClickEvent* ev) {
        if (!rightClickInterface.active || !rightClickInterface.mouseIsInRect(ev)) {
            rightClickInterface.setPosition(ev->pos);
            VTile tileClicked = converter.getPositionInGame(ev->pos);
            Tile* t = map.getTileFromVTile(tileClicked);
            if (t)
                t->onRightClick(*ev);
        }
        });
    MouseRightClickEvent::subscribe(x3);

    auto x4 = new EventObserver<MouseMiddleClickEvent>([&](MouseMiddleClickEvent* ev) {
        clickedOnRightClickInterface = rightClickInterface.active && rightClickInterface.mouseIsInRect(ev);
        if (clickedOnRightClickInterface)
            clickedOnWorld = false;
        clickedOnRightBanner = rightBanner.mouseIsInRect(ev);
        if (clickedOnRightBanner)
            clickedOnWorld = false;
        if (clickedOnWorld) {
            VTile tileClicked = converter.getPositionInGame(ev->pos);
            Tile* t = map.getTileFromVTile(tileClicked);
            if (t)
                t->onMiddleClick(*ev);
        }
        });
    MouseMiddleClickEvent::subscribe(x4);

    auto x5 = new EventObserver<MouseMoveEvent>([&](MouseMoveEvent* ev) {
        // add top left indicator of what your mouse is over + left click option + options.length
        if (rightClickInterface.active && !rightClickInterface.mouseIsInRect(ev)) {
            rightClickInterface.active = false;
        }
        });
    MouseMoveEvent::subscribe(x5);

    while (window.isOpen()) {
        auto dt = clock.getElapsedTime().asMilliseconds();
        if (dt > 1100.f / 60)
            cout << "frame took " << dt << " ms" << endl;
        clock.restart();
        ++frame;
        tickmod = frame % unsigned(Measures::framesPerTick);
        isGameTick = !tickmod;
        if (isGameTick) {
            GameTick::tick();
            taskManager.executeAndRemove();
            player.onGameTick();
            map.shouldUpdate = true;
        }

        sf::Event event;
        clickedOnRightClickInterface = false;
        clickedOnRightBanner = false;
        clickedOnWorld = true;
        // clickedOnBottomBanner = false;

        while (window.pollEvent(event))
            if (event.type == Event::Closed)
                window.close();
            else if (event.type == Event::KeyPressed)
                switch (event.text.unicode) {
                case 71:
                    measures.angle -= 5;
                    break;
                case 72:
                    measures.angle += 5;
                    break;
                }
            else if (event.type == Event::MouseButtonPressed) {
                switch (event.mouseButton.button) {
                default:
                case Left:
                    MouseLeftClickEvent(VPixel(event.mouseButton.x, event.mouseButton.y)).emit();
                    break;
                case Right:
                    MouseRightClickEvent(VPixel(event.mouseButton.x, event.mouseButton.y)).emit();
                    break;
                case Middle:
                    MouseMiddleClickEvent(VPixel(event.mouseButton.x, event.mouseButton.y)).emit();
                    break;
                }
            }
            else if (event.type == Event::Resized)
                measures.update();
            else if (event.type == Event::MouseWheelMoved)
                MouseWheelEvent(VPixel(event.mouseWheel.x, event.mouseWheel.y), event.mouseWheel.delta).emit();
            else if (event.type == Event::MouseMoved)
                MouseMoveEvent(VPixel(event.mouseMove.x, event.mouseMove.y)).emit();

        rightBanner.update();
        player.update(tickmod);
        window.clear();

        map.draw();
        player.draw(camera.getPosition());

        bottomBanner.draw();
        rightBanner.draw();
        rightClickInterface.draw();

        window.display();
    }
    map.shouldStop = true;
    t.join();
}
