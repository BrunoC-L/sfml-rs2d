#include "print.h"
#include <mutex>
#include "logger.h"

static std::mutex m;

void print(const std::ostringstream& ss) {
	std::lock_guard<std::mutex> lock(m);
	std::cout << ss.str();
}

void print(const std::string& s) {
	std::lock_guard<std::mutex> lock(m);
	std::cout << s;
}
