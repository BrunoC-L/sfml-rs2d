#pragma once
#include "mouseEvent.h"

MouseEvent::MouseEvent(VPixel position) : position(position) { positionInGame = VTile();/*getPositionInGame(position);*/ }

MouseClickEvent::MouseClickEvent(sf::Event SFMouseEvent) : MouseEvent(VPixel(SFMouseEvent.mouseButton.x, SFMouseEvent.mouseButton.y)) { };

MouseLeftClickEvent::MouseLeftClickEvent(sf::Event SFMouseEvent) : MouseClickEvent(SFMouseEvent) { };
void MouseLeftClickEvent::accept(MouseEventVisitor* visitor) { visitor->onLeftClick(*this); };

MouseRightClickEvent::MouseRightClickEvent(sf::Event SFMouseEvent) : MouseClickEvent(SFMouseEvent) { };
void MouseRightClickEvent::accept(MouseEventVisitor* visitor) { visitor->onRightClick(*this); };

MouseMiddleClickEvent::MouseMiddleClickEvent(sf::Event SFMouseEvent) : MouseClickEvent(SFMouseEvent) { };
void MouseMiddleClickEvent::accept(MouseEventVisitor* visitor) { visitor->onMiddleClick(*this); };

MouseMoveEvent::MouseMoveEvent(sf::Event SFMouseEvent) : MouseEvent(VPixel(SFMouseEvent.mouseMove.x, SFMouseEvent.mouseMove.y)) { };
void MouseMoveEvent::accept(MouseEventVisitor* visitor) { visitor->onMove(*this); };
