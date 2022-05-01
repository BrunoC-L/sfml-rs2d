#include "chat.h"

Chat::Chat(ServiceProvider* provider) : Service(provider) {
	provider->set(CHAT, this);
	/*messages.push_back(ChatMessage({ {"Hello World!", Color::BLUE } }, ChatMessageType::PlayerMessage));
	messages.push_back(ChatMessage({ {"This is PERFECT", Color::RED }, {"This is PERFECT", Color::BLACK }, {"This is PERFECT", Color::BLUE } }, ChatMessageType::GameMessage));
	messages.push_back(ChatMessage({ {"This is PERFECT", Color::RED } }, ChatMessageType::GameMessage));
	messages.push_back(ChatMessage({ {"This is PERFECT", Color::RED } }, ChatMessageType::GameMessage));
	messages.push_back(ChatMessage({ {"This is PERFECT", Color::RED } }, ChatMessageType::GameMessage));
	messages.push_back(ChatMessage({ {"This is PERFECT", Color::RED } }, ChatMessageType::GameMessage));
	messages.push_back(ChatMessage({ {"This is PERFECT", Color::RED } }, ChatMessageType::GameMessage));*/

	keyObserver.set([&](const LetterKeyPressedEvent::Data& ev) {
		if (!gameData->userIsLoggedIn())
			return;
		currentlyTyped += ev.letter;
	});

	spaceObserver.set([&](const SpaceKeyPressedEvent::Data& ev) {
		if (!gameData->userIsLoggedIn())
			return;
		currentlyTyped += ' ';
	});

	enterObserver.set([&](const EnterKeyPressedEvent::Data& ev) {
		if (!gameData->userIsLoggedIn())
			return;
		JSON message;
		message["message"] = currentlyTyped;
		socket->send("chat", message);
		currentlyTyped = "";
	});

	backspaceObserver.set([&](const BackspaceKeyPressedEvent::Data& ev) {
		if (!gameData->userIsLoggedIn())
			return;
		currentlyTyped = currentlyTyped.substr(0, currentlyTyped.length() - 1);
	});

	loginObserver.set([&](const LoginEvent::Data& ev) {
		currentlyTyped = "";
		messages = {};
	});
}

void Chat::init() {
	acquire();

	socket->on("chat", [&](const std::shared_ptr<const JSON> json) {
		messages.push_back(
			ChatMessage(
				{
					{ json->find("sender") != json->getProperties().end() ? json->get("sender").asString() + ": " : "", Color::BLACK},
					{json->get("message").asString(), Color::BLUE }
				},
				ChatMessageType::PlayerMessage
			)
		);
	});
}

const std::vector<ChatMessage>& Chat::getMessages() {
	return messages;
}

const std::string& Chat::getCurrentMessage() {
	return currentlyTyped;
}