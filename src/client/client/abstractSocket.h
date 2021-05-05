#pragma once
#include "abstractService.h"
#include "json.h"
#include <functional>

class AbstractSocket : public AbstractService {
protected:
	virtual void receive(const std::string& type, std::shared_ptr<const JSON> data) = 0;
public:
	//virtual void send(const std::string& message) = 0;
	virtual void send(const JSON& message) = 0;
	virtual void send(const std::string& type, const JSON& data) = 0;
	virtual void on(const std::string& type, std::function<void(const std::shared_ptr<const JSON>&)>) = 0;
	virtual bool connect() = 0;
	virtual void disconnect() = 0;
	virtual void stop() = 0;
};
