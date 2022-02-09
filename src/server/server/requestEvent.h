#pragma once
#include "event.h"
#include <functional>

template <typename T>
class RequestEvent : public Event {
public:
	std::function<void(T)> callback;
};

template <>
class RequestEvent<void> : public Event {
public:
	std::function<void()> callback;
};