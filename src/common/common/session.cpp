#include "session.h"
#include "getenv.h"
#include <ctime>
#include <sstream>
#include <fstream>

JSON session;

std::string strtime() {
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
			throw std::runtime_error("Why the hell does config.json have more than 3999 characters?");
		session = std::move(JSON(buf));
		session.reserve(15);
		session["name"] = strtime();
		session["RS2D_HOME"] = env;
	}
	return session;
}

const JSON& getSession() {
	return getEditableSession();
}
