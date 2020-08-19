#pragma once
#include <SFML/Graphics.hpp>
#include "units.h"
#include "measures.h"
#include "getRenderWindow.h"
#include "debug.h"
#include "mouseEvent.h"

using namespace std;
using namespace sf;

class RightClickInterface {
public:
    RightClickInterface(const RightClickInterface& other) = delete;
    RightClickInterface operator=(const RightClickInterface& other) = delete;
    static RightClickInterface& getInstance() {
        static RightClickInterface instance;
        return instance;
    }
    void draw();
    bool active;
    void setPosition(VPixel pos);
    bool mouseIsInRect(shared_ptr<MouseEvent> mouseEvent);
    void setText(string text);
    void resetText();
    void click(shared_ptr<MouseEvent> mouseEvent);
private:
    Text text;
    Font font;
    RightClickInterface();
    RectangleShape rect;
};
