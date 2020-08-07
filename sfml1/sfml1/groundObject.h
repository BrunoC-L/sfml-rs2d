#pragma once
#include <SFML/Graphics.hpp>
#include "measures.h"

using namespace sf;
using namespace std;

class GroundObject {
public:
	GroundObject(RectangleShape shape) {
		this->shape = shape;
	}
	void draw(RenderWindow& w, Transform& transform) {
		w.draw(shape, transform);
	}
private:
	RectangleShape shape;
};