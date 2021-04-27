#include "main.h"

int main(int argc, char* argv[]) {
	JSON args;
	for (int i = 1; i < argc; ++i) {
		std::string s(argv[i]);
		auto keyValue = split(s, "=");
		auto key = keyValue[0];
		auto value = keyValue.size() == 2 ? keyValue[1] : "true"; // if an arg is just there without 'arg=value', just 'arg'
		args[key] = value;

		if (keyValue.size() > 2)
			throw std::exception(("split argument '" + s + "' into more than 2 parts\n").c_str());
		if (key.length() == 0)
			throw std::exception(("Key was none for argument '" + s + "'\n").c_str());
		if (value.length() == 0)
			throw std::exception(("Value was none for argument '" + s + "'\n").c_str());
	}

	auto local = args["local"].asBool();
	std::string ip = args["ip"].asBool() ? args["ip"].asString() : local ? "localhost" : "35.182.111.227";
	auto port = args["port"].asBool() ? args["port"].asInt() : 38838;

	ServiceProvider* provider = new ServiceProvider();
	AbstractSocket* socket = new Socket(provider, ip, port);
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
