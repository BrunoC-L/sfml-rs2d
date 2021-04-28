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
	auto radius = args["radius"].asBool() ? args["radius"].asInt() : 1; // if someone wants radius 0 this will fail lol
	ServiceProvider provider;
	Socket s(&provider, ip, port);
	Measures me(&provider);
	Chat chat(&provider);
	Camera camera(&provider);
	Player player(&provider);
	Map map(&provider, radius);
	Inventory i(&provider);
	ClockGameTickProgress cgtp;
	GameDataService g(&provider, &cgtp);

	sf::RenderWindow sfWindow(
		sf::VideoMode(
			AbstractMeasures::startingScreenSize().x,
			AbstractMeasures::startingScreenSize().y
		),
		"RS2D"
	);
	SFRenderWindow window(&provider, sfWindow);

	App app(&provider, &window);

    app.init();
    app.start();
}
