#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <functional>

namespace Logging {
	void log(const std::string& filename, const std::string& text);
	void log(const std::string& filename, const std::stringstream& text);
	std::function<void(const std::string&)> logger(const std::string& filename, bool addNewLines);
	std::function<void(const std::string&)> defaultFolderLogger(const std::string& basepath, const std::string& filename, bool addNewLines);
	std::function<void(const std::string&)> clientDefaultFolderLogger(const std::string& filename, bool addNewLines);
	std::function<void(const std::string&)> serverDefaultFolderLogger(const std::string& filename, bool addNewLines);

	namespace Server {
		void log_default(const std::string&);
	}
	namespace Client {
		void log_default(const std::string&);
	}
}
