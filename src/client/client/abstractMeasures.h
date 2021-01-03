#pragma once
#include "SFML/Graphics.hpp"
#include "units.h"
#include "item.h"
#include <functional>
#include "abstractService.h"

using namespace std;

enum MouseClickType { Left, Right, Middle };

class AbstractMeasures : public AbstractService {
public:
	virtual void setGetWindowSize(function<VPixel()> f) = 0;
	virtual void update() = 0;

	virtual VTile getInnerWindowSizeTile() const = 0;
	virtual VPixel getInnerWindowSize() const = 0;

	virtual sf::Vector2f getTileSize() const = 0;
	virtual float getRightBannerStartingX() const = 0;
	virtual float getBottomBannerStartingY() const = 0;

	static VTile  startingInnerScreenSizeTile() { return VTile(35, 23); }
	static VPixel startingInnerScreenSize() { return VPixel(startingInnerScreenSizeTile() * pixelsPerTile); }
	static VPixel banners() { return VPixel(rightBannerWidth, bottomBannerHeight); }
	static VPixel startingScreenSize() { return VPixel(startingInnerScreenSizeTile().x * pixelsPerTile, startingInnerScreenSizeTile().y * pixelsPerTile) + banners(); }
	function<VPixel()> getWindowSize;
	sf::Vector2f stretch = sf::Vector2f(1, 1);
	float angle = 0;
	float zoom = 1;
	static constexpr float rightBannerWidth = 230;
	static constexpr float bottomBannerHeight = 180;
	static constexpr float TilesPerChunk = 64;
	static constexpr float pixelsPerTileOnMap = 4;
	static constexpr float minimapRadius = 100;
	static constexpr float pixelsPerTile = 32;
	static constexpr float framesPerTick = 60;
	static constexpr float itemsSizePx = 128;
	static constexpr float itemsSizeInInventory = 55;
};
