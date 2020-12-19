#pragma once
#include <SFML/Graphics.hpp>
#include "units.h"
#include "measures.h"
#include "getRenderWindow.h"
#include "mouseEvent.h"
#include "player.h"

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
    bool mouseIsInRect(MouseEvent* ev);
    VPixel getDeltaClick(MouseEvent* ev);
    void addText(string text);
    void resetText();
    void click(MouseEvent* ev);
    void addInteractions(string objectName, vector<pair<string, function<bool()>>> interactions);
private:
    vector<pair<string,function<bool()>>> interactions;
    Text text;
    Font font;
    RightClickInterface();
    RectangleShape rect;
};
