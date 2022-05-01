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
			throw std::runtime_error("split argument '" + s + "' into more than 2 parts\n");
		if (key.length() == 0)
			throw std::runtime_error("Key was none for argument '" + s + "'\n");
		if (value.length() == 0)
			throw std::runtime_error("Value was none for argument '" + s + "'\n");
	}

	auto local = args["local"].asBool();
	std::string ip = args["ip"].asBool() ? args["ip"].asString() : local ? "localhost" : "35.182.111.227";
	auto port = args["port"].asBool() ? args["port"].asInt() : 38838;
	ClientServiceProvider provider;
	Socket s(&provider, ip, port);
	Measures me(&provider);
	Chat chat(&provider);
	Camera camera(&provider);
	Player player(&provider);
	Map map(&provider);
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
	app.stop();
}
