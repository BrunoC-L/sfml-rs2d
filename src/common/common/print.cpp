#include "print.h"
#include <mutex>

static std::mutex m;

void print(std::ostringstream& ss) {
	m.lock();
	std::cout << ss.str();
	m.unlock();
}
