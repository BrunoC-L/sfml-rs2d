#include "logger.h"
#include <unordered_map>
#include <mutex>
#include <filesystem>
#include "session.h"

std::unordered_map<std::string, std::mutex> mutices = {};

void log(std::ofstream& out, const std::string& text) {
	out.write(text.c_str(), text.length());
}

void log(std::string filename, const std::string& text) {
	std::lock_guard<std::mutex> lock(mutices[filename]);
	std::ofstream out(filename, std::ios::app);
	if (!out.is_open()) {
		throw std::exception("Folder missing\n");
	}
	log(out, text);
	out.close();
}

// convert ss to s

void log(std::string filename, const std::stringstream& text) {
	log(filename, text.str());
}

std::function<void(const std::string& text)> logger(std::string filename, bool addNewLines) {
	if (addNewLines)
		return [&, filename](const std::string& text) {
			log(filename, text + "\n");
		};
	else
		return [&, filename](const std::string& text) {
			log(filename, text);
		};
}

std::function<void(const std::string& text)> defaultFolderLogger(std::string basepath, std::string filename, bool addNewLines) {
	auto path = basepath + "/" + getSession().get("name").asString();
	std::filesystem::create_directories(path);
	return logger(path + "/" + filename, addNewLines);
}

void log(std::ofstream& out, const std::stringstream& text) {
	log(out, text.str());
}
