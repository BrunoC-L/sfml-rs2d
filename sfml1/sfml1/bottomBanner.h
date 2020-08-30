#pragma once
#include <SFML/Graphics.hpp>
#include "units.h"
#include "measures.h"
#include "getRenderWindow.h"
using namespace std;
using namespace sf;

class BottomBanner {
public:
    BottomBanner(BottomBanner& other) = delete;
    BottomBanner operator=(const BottomBanner& other) = delete;
    static BottomBanner& getInstance() {
        static BottomBanner instance;
        return instance;
    }
    void draw() const;
private:
    BottomBanner();
    RectangleShape banner;
};