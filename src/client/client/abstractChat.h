#pragma once
#include "abstractService.h"
#include "chatMessage.h"
#include <vector>

class AbstractChat : public AbstractService {
public:
	virtual const std::vector<ChatMessage>& getMessages() = 0;
	virtual const std::string& getCurrentMessage() = 0;
};
