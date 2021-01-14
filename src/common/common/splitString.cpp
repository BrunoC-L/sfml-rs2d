#include "pch.h"
#include "splitstring.h"

vector<string> split(const string& s, const string& divider) {
    if (divider.length() == 0)
        throw new exception("Tried splitting with empty divider");
    string remainder = s;
    vector<string> v = {};
    int index = 0;
    while ((index = remainder.find(divider)) != -1) {
        v.push_back(remainder.substr(0, index));
        remainder = remainder.substr(index + divider.length());
    }
    if (remainder.length())
        v.push_back(remainder);
    return v;
}
