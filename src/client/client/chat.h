#pragma once
#include "abstractServices.h"
#include "service.h"
#include "keyPressedEvent.h"
#include "login.h"

class Chat : public AbstractChat, public Service {
	std::vector<ChatMessage> messages;
	std::string currentlyTyped;
	EventObserver<LetterKeyPressedEvent> keyObserver;
	EventObserver<SpaceKeyPressedEvent> spaceObserver;
	EventObserver<EnterKeyPressedEvent> enterObserver;
	EventObserver<BackspaceKeyPressedEvent> backspaceObserver;
	EventObserver<LoginEvent> loginObserver;

public:
	Chat(ServiceProvider* provider);
	void init();
	virtual const std::vector<ChatMessage>& getMessages() override;
	virtual const std::string& getCurrentMessage() override;
};
