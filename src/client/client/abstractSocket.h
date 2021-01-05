#pragma once
#include "SFML/Graphics.hpp"
#include "abstractService.h"
#include "../../common/json.h"
#include <functional>

using namespace std;

class AbstractSocket : public AbstractService {
public:
	virtual void send(JSON json) = 0;
	virtual void on(std::string type, std::function<void(JSON)>) = 0;
	virtual void emit(std::string, JSON) = 0;
	virtual void receive(std::string, JSON) = 0;
};
