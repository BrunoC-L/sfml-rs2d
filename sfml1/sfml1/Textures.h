#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

class Textures {
public:
	Textures() {
		wallTexture.loadFromFile("../../assets/wallstextures.png");
	}
	Texture wallTexture;
};