#include "pch.h"
#include "splitString.h"

std::vector<std::string> split(const std::string& s, const std::string& divider) {
    if (divider.length() == 0)
        throw std::runtime_error("Tried splitting with empty divider");
    std::string remainder = s;
    std::vector<std::string> v = {};
    int index = 0;
    while ((index = remainder.find(divider)) != -1) {
        v.push_back(remainder.substr(0, index));
        remainder = remainder.substr(index + divider.length());
    }
    if (remainder.length())
        v.push_back(remainder);
    return v;
}
