#pragma once
#include "abstractServices.h"
#include "service.h"

class Chat : public AbstractChat, public Service {
	std::vector<ChatMessage> messages;
public:
	Chat(ServiceProvider* provider);
	void init();
	virtual const std::vector<ChatMessage>& getMessages() override;
};
