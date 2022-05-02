#pragma once
#include "abstractServices.h"
#include "service.h"
#include "keyPressedEvent.h"
#include "login.h"

class Chat : public AbstractChat, public Service {
	std::vector<ChatMessage> messages;
	std::string currentlyTyped;
	LetterKeyPressedEvent::Observer keyObserver;
	SpaceKeyPressedEvent::Observer spaceObserver;
	EnterKeyPressedEvent::Observer enterObserver;
	BackspaceKeyPressedEvent::Observer backspaceObserver;
	LoginEvent::Observer loginObserver;

public:
	Chat(ServiceProvider* provider);
	void init();
	virtual const std::vector<ChatMessage>& getMessages() override;
	virtual const std::string& getCurrentMessage() override;
};
