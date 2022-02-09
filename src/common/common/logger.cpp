#include "logger.h"
#include <unordered_map>
#include <mutex>
#include <filesystem>
#include "session.h"
#include "getenv.h"

std::unordered_map<std::string, std::mutex> mutices = {};

void log(std::ofstream& out, const std::string& text) {
	out.write(text.c_str(), text.length());
}

void log(std::string filename, const std::string& text) {
	std::lock_guard<std::mutex> lock(mutices[filename]);
	std::ofstream out(filename, std::ios::app);
	if (!out.is_open()) {
		throw std::runtime_error("Folder missing\n");
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
	while (true) {
		auto replaceAt = path.find("${");
		if (replaceAt == std::string::npos)
			break;
		auto replaceTo = path.find("}");
		auto env = path.substr(replaceAt + 2, replaceTo - replaceAt - 2);
		auto value = mygetenv(env);
		path = path.replace(replaceAt, replaceTo + 1 - replaceAt, value);
	}
	std::filesystem::create_directories(path);
	return logger(path + "/" + filename, addNewLines);
}

std::function<void(const std::string& text)> clientDefaultFolderLogger(std::string filename, bool addNewLines) {
	return defaultFolderLogger(getSession().get("logs").get("client").asString(), filename, addNewLines);
}

std::function<void(const std::string& text)> serverDefaultFolderLogger(std::string filename, bool addNewLines) {
	return defaultFolderLogger(getSession().get("logs").get("server").asString(), filename, addNewLines);
}

void log(std::ofstream& out, const std::stringstream& text) {
	log(out, text.str());
}
