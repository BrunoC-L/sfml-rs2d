#pragma once

class AbstractRenderWindow;

class InnerRightBanner {
public:
	virtual void draw(AbstractRenderWindow* window) = 0;
};
