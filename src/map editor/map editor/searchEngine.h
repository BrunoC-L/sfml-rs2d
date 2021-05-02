#pragma once
#include "keyPressedEvent.h"
#include <string>
#include <SFML/Graphics.hpp>
#include "units.h"
#include "mouseLeftClickEvent.h"
#include "fileEditor.h"

class SearchEngine {
private:
	EventObserver<LetterKeyPressedEvent> letterObserver;
	EventObserver<EnterKeyPressedEvent> enterObserver;
	EventObserver<BackspaceKeyPressedEvent> backspaceObserver;
	EventObserver<MouseLeftClickEvent> leftClickObserver;
	sf::RectangleShape banner;
	int bottomBannerHeight = 300;
	std::shared_ptr<sf::RenderWindow> window;
	std::string currentSearch;
	std::vector<std::pair<editor::FileEditor::ObjectType, std::string>> results;
	std::vector<std::pair<editor::FileEditor::ObjectType, std::string>> alreadyAdded;
	void updateResults();
	sf::Font& font;
	sf::Text currentSearchText;
	sf::Text resultsText;
	sf::Text alreadyAddedText;
	bool& drawn;
	VPixel& stretch;
	void updateAdded();
public:
	SearchEngine(std::shared_ptr<sf::RenderWindow> window, sf::Font& font, bool& drawn, VPixel& stretch);
	void draw();
	const std::vector<std::pair<editor::FileEditor::ObjectType, std::string>>& getAdded();
	void reset();
	void set(std::vector<std::string>* values);
};
