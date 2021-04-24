#include "main.h"

int main() {
	ServiceProvider* provider = new ServiceProvider();
	AbstractSocket* socket = new Socket(provider, "127.0.0.1", 38838);
	AbstractMeasures* measures = new Measures(provider);
	AbstractChat* chat = new Chat(provider);
	AbstractCamera* camera = new Camera(provider);
	AbstractPlayer* player = new Player(provider);
	AbstractMap* map = new Map(provider);
	AbstractInventory* inventory = new Inventory(provider);
	GameTickProgress* tracker = new ClockGameTickProgress();
	AbstractGameDataService* gameData = new GameDataService(provider, tracker);

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
