#include "sfRenderWindow.h"
#include "closeEvent.h"
#include "tileMap.h"
#include "anchorTransform.h"
#include "font.h"
#include "textures.h"
#include "session.h"

SFRenderWindow::SFRenderWindow(
	ServiceProvider* provider,
	sf::RenderWindow& window
) : Service(provider),
	converter(provider),
	window(window)
{
	font.loadFromFile(getSession().get("RS2D_HOME").asString() + "/assets/runescape_uf.ttf");
	clanTabButtonTexture.loadFromFile(getSession().get("RS2D_HOME").asString() + "/assets/buttons/tabButtons/clan.png");
	friendsTabButtonTexture.loadFromFile(getSession().get("RS2D_HOME").asString() + "/assets/buttons/tabButtons/friends.png");
	ignoreTabButtonTexture.loadFromFile(getSession().get("RS2D_HOME").asString() + "/assets/buttons/tabButtons/ignore.png");
	logoutTabButtonTexture.loadFromFile(getSession().get("RS2D_HOME").asString() + "/assets/buttons/tabButtons/logout.png");
	settingsTabButtonTexture.loadFromFile(getSession().get("RS2D_HOME").asString() + "/assets/buttons/tabButtons/settings.png");
	emotesTabButtonTexture.loadFromFile(getSession().get("RS2D_HOME").asString() + "/assets/buttons/tabButtons/emotes.png");
	musicTabButtonTexture.loadFromFile(getSession().get("RS2D_HOME").asString() + "/assets/buttons/tabButtons/music.png");
	combatTabButtonTexture.loadFromFile(getSession().get("RS2D_HOME").asString() + "/assets/buttons/tabButtons/combat.png");
	skillsTabButtonTexture.loadFromFile(getSession().get("RS2D_HOME").asString() + "/assets/buttons/tabButtons/skills.png");
	questsTabButtonTexture.loadFromFile(getSession().get("RS2D_HOME").asString() + "/assets/buttons/tabButtons/quests.png");
	inventoryTabButtonTexture.loadFromFile(getSession().get("RS2D_HOME").asString() + "/assets/buttons/tabButtons/inventory.png");
	equipmentTabButtonTexture.loadFromFile(getSession().get("RS2D_HOME").asString() + "/assets/buttons/tabButtons/equipment.png");
	prayersTabButtonTexture.loadFromFile(getSession().get("RS2D_HOME").asString() + "/assets/buttons/tabButtons/prayers.png");
	magicTabButtonTexture.loadFromFile(getSession().get("RS2D_HOME").asString() + "/assets/buttons/tabButtons/magic.png");

	leftClickObserver.set([&](const MouseLeftClickEvent::Data& ev) {
		if (gameData->userIsLoggedIn()) {
			bool clickedOnRightClickInterface = rightClickInterface->active && rightClickInterface->mouseIsInRect(ev);
			if (clickedOnRightClickInterface) {
				rightClickInterface->click(ev);
				return;
			}

			bool clickedOnRightBanner = rightBanner->mouseIsInRect(ev);
			if (clickedOnRightBanner) {
				rightBanner->click(ev);
				return;
			}

			bool clickedOnBottomBanner = bottomBanner->mouseIsInRect(ev);
			if (clickedOnBottomBanner) {
				bottomBanner->click(ev);
				return;
			}

			VTile tileClicked = converter.getPositionInGame(ev.pos);
			std::shared_ptr<Tile> t = map->getTileFromVTile(tileClicked);
			if (t)
				t->onLeftClick(ev);
		}
		else {
			auto abs = ev.pos / measures->stretch;
			int newUserButton[2][2] = { {407, 659}, {495, 565} };
			bool clickedOnNewUser =
				abs.x > newUserButton[0][0] &&
				abs.x < newUserButton[0][1] &&
				abs.y > newUserButton[1][0] &&
				abs.y < newUserButton[1][1];

			if (clickedOnNewUser) {
				player->signUp();
				return;
			}

			int existingUserButton[2][2] = { {690, 942}, {495, 565} };
			bool clickedOnExistingUser =
				abs.x > existingUserButton[0][0] &&
				abs.x < existingUserButton[0][1] &&
				abs.y > existingUserButton[1][0] &&
				abs.y < existingUserButton[1][1];

			if (clickedOnExistingUser) {
				player->login();
				return;
			}
		}
	});

	rightClickObserver.set([&](const MouseRightClickEvent::Data& ev) {
		if (gameData->userIsLoggedIn()) {
			if (!rightClickInterface->active || !rightClickInterface->mouseIsInRect(ev)) {
				rightClickInterface->setPosition(ev.pos);
				VTile tileClicked = converter.getPositionInGame(ev.pos);
				std::shared_ptr<Tile> t = map->getTileFromVTile(tileClicked);
				if (t)
					t->onRightClick(ev);
			}
		}
	});

	middleClickObserver.set([&](const MouseMiddleClickEvent::Data& ev) {
		if (gameData->userIsLoggedIn()) {
			auto clickedOnRightClickInterface = rightClickInterface->active && rightClickInterface->mouseIsInRect(ev);
			if (clickedOnRightClickInterface)
				return;
			auto clickedOnRightBanner = rightBanner->mouseIsInRect(ev);
			if (clickedOnRightBanner)
				return;
			VTile tileClicked = converter.getPositionInGame(ev.pos);
			std::shared_ptr<Tile> t = map->getTileFromVTile(tileClicked);
			if (t)
				t->onMiddleClick(ev);
		}
	});

	mouseMoveObserver.set([&](const MouseMoveEvent::Data& ev) {
		if (gameData->userIsLoggedIn()) {
			// add top left indicator of what your mouse is over + left click option + options.length
			if (rightClickInterface->active && !rightClickInterface->mouseIsInRect(ev)) {
				rightClickInterface->active = false;
			}
		}
	});

	resizeObserver.set([&](const ResizeEvent::Data& ev) {
		updateWindowSize();
	});
	
	loginObserver.set([&](const LoginEvent::Data& ev) {
		rightBanner = std::make_shared<RightBanner>(this->provider, this);
		bottomBanner = std::make_shared<BottomBanner>(this->provider, this);
		rightClickInterface = std::make_shared<RightClickInterface>(this->provider, this);
		signUpButton = nullptr;
		loginButton = nullptr;
	});

	logoutObserver.set([&](const LogoutEvent::Data& ev) {
		rightBanner = nullptr;
		bottomBanner = nullptr;
		rightClickInterface = nullptr;

		signUpButton = std::make_shared<Button>(std::make_shared<AnchoredOffsetTransform>(MiddleAnchorTransform, VPixel(-300, 0)), VPixel(200, 100), sf::Color::Yellow);
		signUpButton->onClick([&]() { player->signUp(); });
		signUpButton->text("Sign Up", sf::Color::Red);

		loginButton = std::make_shared<Button>(std::make_shared<AnchoredOffsetTransform>(MiddleAnchorTransform, VPixel(100, 0)), VPixel(200, 100), sf::Color::Yellow);
		loginButton->onClick([&]() { player->login(); });
		loginButton->text("Login", sf::Color::Red);
	});
}

void SFRenderWindow::init() {
	acquire();
	setFramerateLimit(60);
	updateWindowSize();

	BottomRightAnchorTransform.setMeasures(measures);
	BottomLeftAnchorTransform.setMeasures(measures);
	TopRightAnchorTransform.setMeasures(measures);
	TopLeftAnchorTransform.setMeasures(measures);
	MiddleAnchorTransform.setMeasures(measures);

	signUpButton = std::make_shared<Button>(std::make_shared<AnchoredOffsetTransform>(MiddleAnchorTransform, VPixel(-300, 0)), VPixel(200, 100), sf::Color::Yellow);
	signUpButton->onClick([&]() { player->signUp(); });
	signUpButton->text("Sign Up", sf::Color::Red);

	loginButton = std::make_shared<Button>(std::make_shared<AnchoredOffsetTransform>(MiddleAnchorTransform, VPixel(100, 0)), VPixel(200, 100), sf::Color::Yellow);
	loginButton->onClick([&]() { player->login(); });
	loginButton->text("Login", sf::Color::Red);

	p_t.loadFromFile(getSession().get("RS2D_HOME").asString() + "/assets/player.png");
	playerSprite = sf::Sprite(p_t);
	float playerScale = PIXELS_PER_TILE / 32.f; // player texture is correctly scaled when PPT = 32
	playerSprite.setScale(playerScale, playerScale);

	loginPage = sf::RectangleShape(sf::Vector2f(measures->windowSize.x, measures->windowSize.y));
	loginTexture.loadFromFile(getSession().get("RS2D_HOME").asString() + "/assets/login.png");
	loginPage.setTexture(&loginTexture);
}

void SFRenderWindow::draw(const sf::VertexArray& v, const sf::RenderStates& s) {
	window.draw(v, s);
}

void SFRenderWindow::draw(const sf::Shape* s, const sf::Transform& t) {
	window.draw(*s, t);
}

void SFRenderWindow::draw(const sf::Shape& s, sf::Transform t) {
	window.draw(s, t);
}

void SFRenderWindow::draw(const sf::Sprite& s, const sf::Transform& t) {
	window.draw(s, t);
}

void SFRenderWindow::draw(const sf::Text& text, const sf::Transform& t) {
	window.draw(text, t);
}

void SFRenderWindow::draw(TileMap* tilemap, const sf::Transform& t) {
	tilemap->draw(*this, t);
}

void SFRenderWindow::draw(VTile pos, double angle, const sf::Sprite& s) {
	const auto zoomScale = measures->zoom;
	const auto scale = measures->stretch;
	const auto cameraPos = camera->getPosition();
	VTile tileDelta(pos.x - cameraPos.x, pos.y - cameraPos.y);
	VPixel delta(tileDelta * PIXELS_PER_TILE);
	const auto transform = sf::RenderStates().transform
		.rotate(angle)
		.scale(1 / scale.x, 1 / scale.y)
		.translate(
			measures->getInnerWindowSizeTile().x * PIXELS_PER_TILE / 2,
			measures->getInnerWindowSizeTile().y * PIXELS_PER_TILE / 2
		)
		.scale(zoomScale, zoomScale)
		.rotate(measures->angle)
		.translate(delta.x, delta.y)
		.rotate(-measures->angle)
		.translate(-(17.f / 16) * measures->getTileSize().x, -1.5 * measures->getTileSize().y);
	draw(s, transform);
}

VPixel SFRenderWindow::getSize() {
	VPixel v;
	auto size = window.getSize();
	v.x = size.x;
	v.y = size.y;
	return v;
}

void SFRenderWindow::setFramerateLimit(int limit) {
	window.setFramerateLimit(limit);
}

bool SFRenderWindow::isOpen() {
	return window.isOpen();
}

void SFRenderWindow::close() {
	if (window.isOpen())
		window.close();
}

void SFRenderWindow::clear() {
	window.clear();
}

void SFRenderWindow::display() {
	window.display();
	EVENT(FrameEvent).emit();
}

void SFRenderWindow::events() {
	sf::Event event;
	while (window.pollEvent(event))
		if (event.type == sf::Event::Closed) {
			EVENT(CloseEvent).emit();
		}
		else if (event.type == sf::Event::KeyPressed)
			switch (event.text.unicode) {
				case 71:
					EVENT(LeftArrowKeyPressedEvent).emit();
					break;
				case 72:
					EVENT(RightArrowKeyPressedEvent).emit();
					break;
				case 58:
					EVENT(EnterKeyPressedEvent).emit();
					break;
				case 59:
					EVENT(BackspaceKeyPressedEvent).emit();
					break;
				case 60:
					EVENT(TabKeyPressedEvent).emit();
					break;
				default:
					int code = event.text.unicode;
					bool isLetter = code < 26;
					if (isLetter) {
						EVENT(LetterKeyPressedEvent, char(97 + code), false).emit();
						break;
					}
					bool isNumber = code < 36;
					if (isNumber) {
						EVENT(LetterKeyPressedEvent, char(22 + code), false).emit();
						break;
					}
			}
		else if (event.type == sf::Event::MouseButtonPressed) {
			switch (event.mouseButton.button) {
			default:
			case Left:
				EVENT(MouseLeftClickEvent, VPixel(event.mouseButton.x, event.mouseButton.y)).emit();
				break;
			case Right:
				EVENT(MouseRightClickEvent, VPixel(event.mouseButton.x, event.mouseButton.y)).emit();
				break;
			case Middle:
				EVENT(MouseMiddleClickEvent, VPixel(event.mouseButton.x, event.mouseButton.y)).emit();
				break;
			}
		}
		else if (event.type == sf::Event::Resized)
			EVENT(ResizeEvent).emit();
		else if (event.type == sf::Event::MouseWheelMoved)
			EVENT(MouseWheelEvent, VPixel(event.mouseWheel.x, event.mouseWheel.y), event.mouseWheel.delta).emit();
		else if (event.type == sf::Event::MouseMoved)
			EVENT(MouseMoveEvent, VPixel(event.mouseMove.x, event.mouseMove.y)).emit();
}

void SFRenderWindow::draw() {
	clear();
	if (gameData->userIsLoggedIn()) {
		update();
		if (map->ready()) {
			auto playerPositions = gameData->getPlayerPositions();
			auto getTransform = [&](const VTile& relativePos, const VChunk& chunkOffset) {
				const float scale = measures->zoom;
				VTile offsetTiles = VTile(chunkOffset.x * TILES_PER_CHUNK, chunkOffset.y * TILES_PER_CHUNK) - relativePos;
				const auto offset = VPixel(PIXELS_PER_TILE * offsetTiles.x * scale, PIXELS_PER_TILE * offsetTiles.y * scale);
				const VTile scalingDiff = measures->getInnerWindowSizeTile() * VTile(1 - scale, 1 - scale);
				const VPixel scalingDiffPx = VPixel(PIXELS_PER_TILE * scalingDiff.x, PIXELS_PER_TILE * scalingDiff.y) / 2;
				const auto finalOffset = offset + scalingDiffPx;

				sf::Transform transform;
				const sf::Vector2f middleOfInnerWindow(
					measures->getInnerWindowSizeTile().x * PIXELS_PER_TILE / 2,
					measures->getInnerWindowSizeTile().y * PIXELS_PER_TILE / 2
				);
				transform.scale(sf::Vector2f(1 / measures->stretch.x, 1 / measures->stretch.y));
				transform.rotate(measures->angle, middleOfInnerWindow);
				transform.translate(sf::Vector2f(finalOffset.x, finalOffset.y));
				transform.scale(scale, scale);
				return transform;
			};
			const VTile pos = camera->getPosition();
			VTile relativePos(
				pos.x - map->getCenterChunk().x * TILES_PER_CHUNK - measures->getInnerWindowSizeTile().x / 2,
				pos.y - map->getCenterChunk().y * TILES_PER_CHUNK - measures->getInnerWindowSizeTile().y / 2
			);
			{
				std::lock_guard<std::mutex> lock(map->getChunksMutex());
				for (int i = 0; i < 2 * CHUNK_RADIUS + 1; ++i)
					for (int j = 0; j < 2 * CHUNK_RADIUS + 1; ++j) {
						auto& chunk = map->getLoaded(i, j);
						sf::Transform transform = getTransform(relativePos + VTile(0.5, 0.5), VChunk(i, j) - VChunk(CHUNK_RADIUS, CHUNK_RADIUS));
						draw(&chunk.map, transform);
						draw(&chunk.objectMap, transform);
					}
			}
			for (int i = 0; i < playerPositions.size(); ++i)
				draw(playerPositions[i].second, 0, playerSprite);
		}

		bottomBanner->draw();
		rightBanner->draw();
		rightClickInterface->draw();
	}
	else {
		auto credentials = player->getUserNamePw();
		sf::Text credentialsText(credentials.first + "\n" + credentials.second, font);
		const auto scale = measures->stretch;
		sf::Transform t(MiddleAnchorTransform.getTransform());
		t.translate(-200, -200);
		draw(credentialsText, t);
		if (signUpButton)
			signUpButton->draw(*this);
		if (loginButton)
			loginButton->draw(*this);
	}
	display();
}

void SFRenderWindow::update() {
	rightBanner->update();
}

void SFRenderWindow::setActive(bool newState = true) {
	window.setActive(newState);
}

void SFRenderWindow::updateWindowSize() {
	auto size = getSize();
	measures->windowSize = size;
	measures->stretch = VPixel(size.x / AbstractMeasures::startingScreenSize().x, size.y / AbstractMeasures::startingScreenSize().y);
}
