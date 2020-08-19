#pragma once
#include <memory>

class MouseLeftClickEvent;
class MouseRightClickEvent;
class MouseMiddleClickEvent;
class MouseMoveEvent;

class MouseEventVisitor {
public:
	virtual function<bool()> onLeftClick(MouseLeftClickEvent event) = 0;
	virtual function<bool()> onRightClick(MouseRightClickEvent event) = 0;
	virtual function<bool()> onMiddleClick(MouseMiddleClickEvent event) = 0;
	virtual function<bool()> onMove(MouseMoveEvent event) = 0;
};
