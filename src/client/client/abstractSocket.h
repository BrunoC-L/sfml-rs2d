#pragma once
#include "../../common/common/abstractService.h"
#include "../../common/common/json.h"
#include <functional>

using namespace std;

class AbstractSocket : public AbstractService {
public:
	virtual void send(JSON& json) = 0;
	virtual void on(std::string type, std::function<void(JSON&)>) = 0;
	virtual void emit(std::string, JSON&) = 0;
	virtual void receive(std::string, JSON&) = 0;
	virtual void login() = 0;
	virtual void connect(std::string ip, unsigned port) = 0;
	virtual void disconnect() = 0;
};
