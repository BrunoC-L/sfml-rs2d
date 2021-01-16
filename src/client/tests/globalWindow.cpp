#include "pch.h"
#include "main.h"
#include "globalWindow.h"

sf::RenderWindow* globalWindow = new sf::RenderWindow(
	sf::VideoMode(
		AbstractMeasures::startingScreenSize().x,
		AbstractMeasures::startingScreenSize().y
	),
	"RS2D"
);
