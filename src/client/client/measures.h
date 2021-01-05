#pragma once
#include "../../common/units.h"
#include <SFML/Graphics.hpp>
#include <functional>
#include <iostream>
#include "abstractServices.h"
#include "keyPressedEvent.h"
#include "resizeEvent.h"
#include "service.h"

using namespace std;

class Measures : public AbstractMeasures, public Service {
public:
	Measures(AbstractServiceProvider* provider) : Service(provider) {
		provider->set("Measures", this);
	}
	void init() {
		acquire();
		setGetWindowSize([&]() { return VPixel(renderWindow->getSize().x, renderWindow->getSize().y); });
		LeftArrowKeyPressedEvent::subscribe(new EventObserver<LeftArrowKeyPressedEvent>([&](LeftArrowKeyPressedEvent* ev) {
			angle -= 5;
			}));
		RightArrowKeyPressedEvent::subscribe(new EventObserver<RightArrowKeyPressedEvent>([&](RightArrowKeyPressedEvent* ev) {
			angle += 5;
		}));
		ResizeEvent::subscribe(new EventObserver<ResizeEvent>([&](ResizeEvent* ev) {
			update();
		}));
	}
	virtual void setGetWindowSize(function<VPixel()> f) { getWindowSize = f; };
	virtual void update() { stretch = sf::Vector2f(getWindowSize().x / AbstractMeasures::startingScreenSize().x, getWindowSize().y / AbstractMeasures::startingScreenSize().y); }

	virtual VTile getInnerWindowSizeTile() const {
		VPixel size = getWindowSize();
		VPixel banners(rightBannerWidth, bottomBannerHeight);
		size -= banners;
		return VTile(size.x, size.y) / 32;
	}
	virtual VPixel getInnerWindowSize() const {
		VPixel size = getWindowSize();
		VPixel banners(rightBannerWidth, bottomBannerHeight);
		return size - banners;
	}

	virtual sf::Vector2f getTileSize() const {
		return sf::Vector2f(pixelsPerTile, pixelsPerTile);
	}
	virtual float getRightBannerStartingX()  const { return AbstractMeasures::startingScreenSize().x - AbstractMeasures::banners().x; }
	virtual float getBottomBannerStartingY() const { return AbstractMeasures::startingScreenSize().y - AbstractMeasures::banners().y; }
};
