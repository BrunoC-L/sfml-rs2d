#pragma once
#include "event.h"
#include "json.h"

struct LoginEventData {
    JSON json;
};
using LoginEvent = Event<LoginEventData>;

struct LogoutEventData {
};
using LogoutEvent = Event<LogoutEventData>;
