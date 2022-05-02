#include "getenv.h"
#include <vector>
#include <stdexcept>

std::vector<std::string> knownEnvKeys;
std::vector<std::string> knownEnvValues;

std::string mygetenv(const std::string& env) {
    auto it = std::find(knownEnvKeys.begin(), knownEnvKeys.end(), env);
    if (it != knownEnvKeys.end())
        return knownEnvValues.at(it - knownEnvKeys.begin());
    char* buf = nullptr;
#ifdef __APPLE__
    buf = getenv(env.c_str());
#else // __APPLE__
    _dupenv_s(&buf, nullptr, env.c_str());
#endif // __APPLE__
    if (buf == nullptr)
        throw std::runtime_error(("No environment variable set for '" + env + "'").c_str());
    knownEnvKeys.push_back(env);
    knownEnvValues.push_back(buf);
    return buf;
}
