#include "logger.h"
#include <unordered_map>
#include <mutex>
#include <thread>
#include <filesystem>
#include "session.h"
#include "getenv.h"
#include "print.h"

namespace Logging {
	std::unordered_map<std::string, std::mutex> mutices = {};

	void log(const std::string& filename, const std::string& text) {
		std::lock_guard<std::mutex> lock(mutices[filename]);
		std::ofstream out(filename, std::ios::app);
		if (!out.is_open()) {
			throw std::runtime_error("Folder missing\n");
		}
		out.write(text.c_str(), text.length());
		out.close();
	}

	void log(const std::string& filename, const std::stringstream& text) {
		log(filename, text.str());
	}

	std::function<void(const std::string& text)> logger(const std::string& filename, bool addNewLines) {
		if (addNewLines)
			return [&, filename](const std::string& text) {
			log(filename, text + "\n");
		};
		else
			return [&, filename](const std::string& text) {
			log(filename, text);
		};
	}

	std::function<void(const std::string& text)> defaultFolderLogger(const std::string& basepath, const std::string& filename, bool addNewLines) {
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

	std::function<void(const std::string& text)> clientDefaultFolderLogger(const std::string& filename, bool addNewLines) {
		return defaultFolderLogger(getSession().get("logs").get("client").asString(), filename, addNewLines);
	}

	std::function<void(const std::string& text)> serverDefaultFolderLogger(const std::string& filename, bool addNewLines) {
		return defaultFolderLogger(getSession().get("logs").get("server").asString(), filename, addNewLines);
	}

	namespace Server {
		void log_default(const std::string& s) {
			static std::function<void(const std::string&)> do_log_default = serverDefaultFolderLogger("/default.txt", true);
			std::stringstream os;
			os << "THREAD_ID: " << std::this_thread::get_id() << ": " << s << "\n";
			std::string os_s(os.str());
			do_log_default(os_s);
			//print(os_s);
		}
	}

	namespace Client {
		void log_default(const std::string& s) {
			static std::function<void(const std::string&)> do_log_default = clientDefaultFolderLogger("/default.txt", true);
			std::stringstream os;
			os << "THREAD_ID: " << std::this_thread::get_id() << ": " << s << "\n";
			std::string os_s(os.str());
			do_log_default(os_s);
			//print(os_s);
		}
	}
}
