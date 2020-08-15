#include "splitString.h"

vector<string> split(const string& s, const char c) {
    vector<string> v = {};
    string buffer = "";
    for (int i = 0; i < s.length(); ++i) {
        char t = s[i];
        if (t == c) {
            v.push_back(buffer);
            buffer = "";
        }
        else
            buffer += t;
    }
    v.push_back(buffer);
    return v;
}
