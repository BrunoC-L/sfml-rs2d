#include "clearevents.h"

void clearAllEventSubscribers() {
	LoginEvent::clear();
	LogoutEvent::clear();
}
