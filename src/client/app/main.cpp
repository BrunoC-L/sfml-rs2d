#include "main.h"
#include "tickScheduler.h"

int main() {
	AbstractServiceProvider* provider = new AbstractServiceProvider();
	AbstractSocket* socket = new Socket(provider);
	AbstractMeasures* measures = new Measures(provider);
	AbstractChat* chat = new Chat(provider);
	AbstractCamera* camera = new Camera(provider);
	AbstractPlayer* player = new Player(provider);
	AbstractMap* map = new Map(provider);
	AbstractInventory* inventory = new Inventory(provider);
	GameTickProgress* tracker = new GameTickProgress();
	GameDataStorage* storage = new GameDataStorage();
	AbstractGameDataService* gameData = new GameDataService(provider, tracker, storage);

	sf::RenderWindow sfWindow(
		sf::VideoMode(
			AbstractMeasures::startingScreenSize().x,
			AbstractMeasures::startingScreenSize().y
		),
		"RS2D"
	);
	AbstractRenderWindow* window = new SFRenderWindow(provider, sfWindow);

	App app(provider, window, socket, measures, map, player, camera, chat, inventory, gameData);

    app.init();
    app.start();
}
