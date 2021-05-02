#include "searchEngine.h"
#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>

SearchEngine::SearchEngine(std::shared_ptr<sf::RenderWindow> window, sf::Font& font, bool& drawn, VPixel& stretch) :
	window(window),
	font(font),
	drawn(drawn),
	stretch(stretch)
{
	currentSearchText.setFont(font);
	currentSearchText.setFillColor(sf::Color::Black);
	currentSearchText.setPosition(sf::Vector2f(window->getSize().x / 3, window->getSize().y - bottomBannerHeight));
	resultsText.setFont(font);
	resultsText.setFillColor(sf::Color::Black);
	resultsText.setPosition(sf::Vector2f(window->getSize().x / 3, window->getSize().y - bottomBannerHeight + 30));
	alreadyAddedText.setFont(font);
	alreadyAddedText.setFillColor(sf::Color::Black);
	alreadyAddedText.setPosition(sf::Vector2f(window->getSize().x * 2 / 3, window->getSize().y - bottomBannerHeight));
	alreadyAddedText.setString("Already Added:");

	banner = sf::RectangleShape(sf::Vector2f(window->getSize().x, bottomBannerHeight));
	banner.setPosition(0, window->getSize().y - bottomBannerHeight);

	letterObserver.set([&](LetterKeyPressedEvent& ev) {
		currentSearch += std::tolower(ev.letter);
		currentSearchText.setString(currentSearch);
		drawn = false;
	});
	enterObserver.set([&](EnterKeyPressedEvent& ev) {
		updateResults();
		drawn = false;
	});
	backspaceObserver.set([&](BackspaceKeyPressedEvent& ev) {
		if (!currentSearch.empty())
			currentSearch.erase(std::prev(currentSearch.end()));
		currentSearchText.setString(currentSearch);
		drawn = false;
	});
	leftClickObserver.set([&](MouseLeftClickEvent& ev) {
		if (ev.pos.y <= this->window->getSize().y - 300 * stretch.y)
			return;
		auto scaled = (ev.pos - VPixel(0, this->window->getSize().y - 300 * stretch.y))/stretch;
		if (scaled.x < resultsText.getPosition().x)
			return;
		scaled.y -= 10;
		int index = int(scaled.y / 30) - 1;
		if (index < 0)
			return;
		std::cout << index << std::endl;
		if (scaled.x < alreadyAddedText.getPosition().x) {
			if (results.size() <= index)
				return;
			if (std::find(alreadyAdded.begin(), alreadyAdded.end(), results[index]) != alreadyAdded.end())
				return;
			alreadyAdded.push_back(results[index]);
		}
		else {
			if (alreadyAdded.size() <= index)
				return;
			alreadyAdded.erase(alreadyAdded.begin() + index);
		}
		updateAdded();
		drawn = false;
	});
}

void SearchEngine::updateResults() {
	results.clear();
	if (currentSearch.length() == 0)
		return;
	std::pair<editor::FileEditor::ObjectType, std::string> typesAndFiles[1] = {
		{editor::FileEditor::ObjectType::OBJECT, "objectNames"}
	};
	for (const auto& typeAndFile : typesAndFiles) {
		std::string fileName = "../../../assets/data/" + typeAndFile.second + ".txt";
		std::ifstream file(fileName);
		std::string line;
		if (!file.is_open())
			throw std::exception(("Failed to open " + fileName).c_str());
		while (std::getline(file, line)) {
			std::string copy = line;
			std::transform(copy.begin(), copy.end(), copy.begin(), [](unsigned char c) { return std::tolower(c); });
			if (copy.find(currentSearch) != std::string::npos)
				results.push_back({ typeAndFile.first, line });
		}
	}
	std::stringstream ss;
	for (const auto& r : results)
		ss << r.second << std::endl;
	resultsText.setString(ss.str());
}

void SearchEngine::draw() {
	window->draw(banner);
	window->draw(currentSearchText);
	window->draw(resultsText);
	window->draw(alreadyAddedText);
}

const std::vector<std::pair<editor::FileEditor::ObjectType, std::string>>& SearchEngine::getAdded() {
	return alreadyAdded;
}

void SearchEngine::reset() {
	alreadyAdded.clear();
	alreadyAddedText.setString("Already Added:");
}

void SearchEngine::set(std::vector<std::string>* values) {
	for (int i = 0; i < editor::FileEditor::SIZE; ++i)
		for (const auto& v : values[i])
			alreadyAdded.push_back({editor::FileEditor::ObjectType(i), v});
	updateAdded();
}

void SearchEngine::updateAdded() {
	std::stringstream ss;
	ss << "Already Added:\n";
	for (const auto& r : alreadyAdded)
		ss << r.second << std::endl;
	alreadyAddedText.setString(ss.str());
}
