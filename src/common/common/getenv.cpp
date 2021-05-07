#include "getenv.h"
#include <unordered_map>

std::unordered_map<std::string, std::string> knownEnv;

std::string mygetenv(const std::string& env) {
	auto it = knownEnv.find(env);
	if (it != knownEnv.end())
		return it->second;
	char* buf = nullptr;
	if (_dupenv_s(&buf, nullptr, env.c_str()) == 0 && buf == nullptr)
		throw std::exception(("No environment variable set for '" + env + "'").c_str());
	knownEnv[env] = std::move(buf);
	return knownEnv[env];
}
