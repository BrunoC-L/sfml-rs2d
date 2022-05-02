#include "pch.h"
#include "main.h"
#include "globalWindow.h"

std::shared_ptr<sf::RenderWindow> globalWindow = std::make_shared<sf::RenderWindow>(
	sf::VideoMode(
		AbstractMeasures::startingScreenSize().x,
		AbstractMeasures::startingScreenSize().y
	),
	"RS2D"
);
