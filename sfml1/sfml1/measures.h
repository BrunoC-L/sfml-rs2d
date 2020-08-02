#pragma once
#include "units.h"
#include <SFML/Graphics.hpp>
#include <functional>
#include <iostream>

using namespace sf;
using namespace std;
using RS = RenderStates;

class Measures {
public:
	Measures() {
		startingInnerScreenSizeTile = VTile(36, 24);
		auto inpx = startingInnerScreenSizeTile * pixelsPerTile;
		startingInnerScreenSize = VPixel(inpx.x, inpx.y);
		banners = VPixel(rightBannerWidth, bottomBannerHeight);
		startingScreenSize = VPixel(startingInnerScreenSizeTile.x * pixelsPerTile, startingInnerScreenSizeTile.y * pixelsPerTile) + banners;
	}

	void setGetWindowSize(std::function<VPixel()> f) { getWindowSize = f; };
	void update() { stretch = Vector2f(getWindowSize().x / startingScreenSize.x, getWindowSize().y / startingScreenSize.y); }
	
	VTile getInnerWindowSizeTile() const { VPixel size = getWindowSize(); VPixel banners(rightBannerWidth, bottomBannerHeight); size -= banners; return VTile(size.x, size.y) / 32; }
	VTile getWindowSizeTile()      const { VPixel size = getWindowSize();  return VTile((int)size.x / 32, (int)size.y / 32); }
	VPixel getPixelsPerTile()      const { return VPixel(pixelsPerTile, pixelsPerTile) / VPixel(stretch.x, stretch.y); }
	
	Vector2f getTileSize()         const { return Vector2f(pixelsPerTile / zoom.x, pixelsPerTile / zoom.y); }
	Vector2f getScale()            const { return Vector2f(zoom); }
	RS getRightBannerRS()          const { auto rs = RS(); rs.transform.scale(Vector2f(1 / stretch.x, 1)); return rs; }
	RS getBottomBannerRS()         const { auto rs = RS(); rs.transform.scale(Vector2f(1, 1 / stretch.y)); return rs; }

	float getRightBannerStartingX()   const { return startingScreenSize.x - banners.x; }
	float getBottomBannerStartingY()  const { return startingScreenSize.y - banners.y; }
	Vector2f getBannerSizes()	   const { return Vector2f(banners.x / stretch.x, banners.y/ stretch.y);}

	VTile  startingInnerScreenSizeTile;
	VPixel startingInnerScreenSize;
	VPixel banners;
	VPixel startingScreenSize;
	std::function<VPixel()> getWindowSize;
	float rightBannerWidth = 230;
	float bottomBannerHeight = 180;
	float angle = 0;
	Vector2f zoom = Vector2f(0.3, 0.3);
	Vector2f stretch = Vector2f(1, 1);
	float TilesPerChunk = 64;
	float pixelsPerTileOnMap = 4;
	float minimapRadius = 100;
	float pixelsPerTile = 32;
};
