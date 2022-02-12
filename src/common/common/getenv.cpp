#include "getenv.h"
#include <unordered_map>
#include <stdexcept>

std::unordered_map<std::string, std::string> knownEnv;

std::string mygetenv(const std::string& env) {
    auto it = knownEnv.find(env);
    if (it != knownEnv.end())
        return it->second;
    char* buf = nullptr;
#ifdef __APPLE__
    buf = getenv(env.c_str());
#else // __APPLE__
    _dupenv_s(&buf, nullptr, env.c_str());
#endif // __APPLE__
    if (buf == nullptr)
        throw std::runtime_error(("No environment variable set for '" + env + "'").c_str());
    knownEnv[env] = buf;
    //delete buf;
    return knownEnv[env];
}
