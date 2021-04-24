#pragma once
#include "abstractService.h"
#include "json.h"
#include <functional>

using namespace std;

class AbstractSocket : public AbstractService {
public:
	virtual void send(JSON& json) = 0;
	virtual void send(std::string str) = 0;
	virtual void sendNoCheck(std::string str) = 0;
	virtual void on(std::string type, std::function<void(JSON&)>) = 0;
	virtual void emit(std::string, JSON&) = 0;
	virtual void receive(std::string, JSON&) = 0;
	virtual bool connect() = 0;
	virtual void disconnect() = 0;
};
