#include "print.h"
#include <mutex>

static std::mutex m;

void print(std::ostringstream& ss) {
	std::lock_guard<std::mutex> lock(m);
	std::cout << ss.str();
}
