#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <functional>

//void log(std::ofstream& out, const std::string& text);
//void log(std::ofstream& out, const std::stringstream& text);
void log(std::string filename, const std::string& text);
void log(std::string filename, const std::stringstream& text);
std::function<void(const std::string& text)> logger(std::string filename, bool addNewLines);
std::function<void(const std::string& text)> defaultFolderLogger(std::string basepath, std::string filename, bool addNewLines);
