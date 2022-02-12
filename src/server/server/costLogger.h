#pragma once
#include <thread>
#include <chrono>
#include <fstream>
#include <ctime>

class CostLogger {
protected:
	std::ofstream out;
	std::chrono::steady_clock::time_point begin;
	std::string timestamp;
public:
	CostLogger(std::string fileName) {
		out.open(fileName, std::ios::app);
		if (!out.is_open())
			throw std::runtime_error("Couldn't open file: " + fileName);
		begin = std::chrono::steady_clock::now();
		struct tm newtime;
		time_t now = time(0);
#ifdef __APPLE__
        struct tm* newtimemac = localtime(&now);
        newtime = *newtimemac;
        delete newtimemac;
#else // __APPLE__
    localtime_s(&newtime, &now);
#endif // __APPLE__
		char buffer[80];
		const auto* ptr = &newtime;
		strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S", ptr);
		timestamp = std::string(buffer);
	}

	~CostLogger() {
		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
		auto total = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
		out << timestamp << " " << total << std::endl;
		out.close();
	}
};
