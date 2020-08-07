#pragma once
#include "units.h"
#include <SFML/Graphics.hpp>
#include <functional>
#include <iostream>

using namespace sf;
using namespace std;

class Measures {
public:
	Measures() {
		startingInnerScreenSizeTile = VTile(35, 23);
		auto inpx = startingInnerScreenSizeTile * pixelsPerTile;
		startingInnerScreenSize = VPixel(inpx.x, inpx.y);
		banners = VPixel(rightBannerWidth, bottomBannerHeight);
		startingScreenSize = VPixel(startingInnerScreenSizeTile.x * pixelsPerTile, startingInnerScreenSizeTile.y * pixelsPerTile) + banners;
	}

	void setGetWindowSize(std::function<VPixel()> f) { getWindowSize = f; };
	void update() { stretch = Vector2f(getWindowSize().x / startingScreenSize.x, getWindowSize().y / startingScreenSize.y); }

	VTile getInnerWindowSizeTile() const { VPixel size = getWindowSize(); VPixel banners(rightBannerWidth, bottomBannerHeight); size -= banners; return VTile(size.x, size.y) / 32; }
	VPixel getInnerWindowSize() const { VPixel size = getWindowSize(); VPixel banners(rightBannerWidth, bottomBannerHeight); return size - banners; }
	
	Vector2f getTileSize()         const { return Vector2f(pixelsPerTile, pixelsPerTile); }

	float getRightBannerStartingX()   const { return startingScreenSize.x - banners.x; }
	float getBottomBannerStartingY()  const { return startingScreenSize.y - banners.y; }

	VTile  startingInnerScreenSizeTile;
	VPixel startingInnerScreenSize;
	VPixel banners;
	VPixel startingScreenSize;
	std::function<VPixel()> getWindowSize;
	float rightBannerWidth = 230;
	float bottomBannerHeight = 180;
	float angle = 0;
	float zoom = 1;
	Vector2f stretch = Vector2f(1, 1);
	static constexpr float TilesPerChunk = 64;
	static constexpr float pixelsPerTileOnMap = 4;
	static constexpr float minimapRadius = 100;
	static constexpr float pixelsPerTile = 32;
};
