#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

class Textures {
	Textures() {
		wallTexture.loadFromFile("../../assets/wallstextures.png");
	}
	Texture wallTexture;
public:
	Textures(const Textures& other) = delete;
	Textures operator=(const Textures& other) = delete;
	static Textures& getInstance() {
		static Textures instance;
		return instance;
	}
};
