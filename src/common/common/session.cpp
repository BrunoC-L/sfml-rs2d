#include "session.h"
#include "getenv.h"
#include <ctime>
#include <sstream>
#include <fstream>

JSON session;

std::string strtime() {
	struct tm newtime;
	time_t now = time(0);
	localtime_s(&newtime, &now);
	char buffer[80];
	const auto* ptr = &newtime;
	strftime(buffer, sizeof(buffer), "%d-%m-%Y-%H-%M-%S", ptr);
	return std::string(buffer);
}

JSON& getEditableSession() {
	if (session.size() == 0) {
		auto env = mygetenv("RS2D_HOME");
		const size_t SIZE = 4000;
		char buf[SIZE] = { 0 };
		std::ifstream f(env + "/config.json");
		bool readEntireFile = f.read(buf, SIZE).gcount() < SIZE;
		if (!readEntireFile)
			throw std::exception("Why the hell does config.json have more than 3999 characters?");
		session = std::move(JSON(buf));
		session["name"] = strtime();
		session["RS2D_HOME"] = env;
	}
	return session;
}

const JSON& getSession() {
	return getEditableSession();
}
