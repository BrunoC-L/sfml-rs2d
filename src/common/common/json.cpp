#include "json.h"
#include <sstream>

JSON::JSON(const std::string& json) {
	self = json;
	parse(self);
}

JSON::JSON() {
	self = "{}";
	parse(self);
}

JSON::JSON(std::string&& json) {
	std::swap(self, json);
	parse(self);
}

JSON::JSON(const std::string& json, std::string propertyName) {
	this->propertyName = propertyName;
	self = json;
	parse(self);
}

JSON::JSON(JSON&& other) {
	std::swap(children, other.children);
	std::swap(indices, other.indices);
	std::swap(self, other.self);
	type = other.type;
	defined = other.defined;
}

void JSON::operator=(const JSON& other) {
	type = other.type;
	self = other.self;
	indices = other.indices;
	children = other.children;
	defined = true;
}

JSON JSON::getInactive(std::string propertyName) {
	JSON json;
	json.defined = false;
	json.propertyName = propertyName;
	return std::move(json);
}

JSON& JSON::operator[](const std::string& propertyName) {
	assertType(Primitives::OBJECT);
	if (!has(propertyName)) {
		children.emplace_back(std::move(getInactive(propertyName)));
		indices.insert(make_pair(propertyName, children.size() - 1));
		properties.push_back(propertyName);
	}
	JSON& child = children[indices.at(propertyName)];
	child.propertyName = propertyName;
	return child;
};

const JSON& JSON::get(const std::string& propertyName) const {
	assertType(Primitives::OBJECT);
	if (!has(propertyName))
		throw JSONException("Missing property, can't provide const reference");
	auto index = indices.at(propertyName);
	return children[index];
}

JSON& JSON::operator[](int x) {
	assertType(Primitives::ARRAY);
	return children[x];
}

unsigned JSON::size() const {
	return children.size();
}

const std::vector<std::string>& JSON::getProperties() const {
	return properties;
}

const std::vector<JSON>& JSON::getChildren() const {
	return children;
}

const std::string& JSON::getName() const {
	return propertyName;
}

bool JSON::has(const std::string& propertyName) const {
	return indices.find(propertyName) != indices.end();
}

const JSON& JSON::get(int x) const {
	assertType(Primitives::ARRAY);
	return children[x];
}

void JSON::push(JSON&& json) {
	assertType(Primitives::ARRAY);
	this->children.emplace_back(json);
}

void JSON::push(const JSON& json) {
	assertType(Primitives::ARRAY);
	this->children.push_back(json);
}

void JSON::push(std::string str) {
	push(JSON(str));
}

std::string JSON::asString() const {
	return asString(0);
}

std::string JSON::asString(unsigned tabulation) const {
	return asString(tabulation, 0);
}

std::string JSON::arrayAsString(unsigned tabulation, int indent) const {
	bool newLines = tabulation > 0;
	std::stringstream out;
	out << "[";
	if (children.size()) {
		if (newLines)
			out << "\n";
		for (int i = 0; i < tabulation * indent; ++i)
			out << " ";
	}
	bool hasChildren = false;
	for (const auto& json : children) {
		if (!json.defined)
			continue;
		if (hasChildren) {
			if (newLines) {
				out << ",\n";
				for (int i = 0; i < tabulation * indent; ++i)
					out << " ";
			}
			else
				out << ", ";
		}
		hasChildren = true;
		switch (json.type) {
			case Primitives::STRING:
				out << '"' + json.asString(tabulation, indent) + '"';
				break;
			default:
				out << json.asString(tabulation, indent);
				break;
		}
	}
	if (hasChildren) {
		if (newLines)
			out << "\n";
		for (int i = 0; i < tabulation * (indent - 1); ++i)
			out << " ";
	}
	out << "]";
	return out.str();
}

std::string JSON::objectAsString(unsigned tabulation, int indent) const {
	bool newLines = tabulation > 0;
	std::stringstream out;
	out << "{";
	if (children.size()) {
		if (newLines)
			out << "\n";
		for (int i = 0; i < tabulation * indent; ++i)
			out << " ";
	}
	bool hasChildren = false;
	for (const auto& propertName : properties) {
		auto index = indices.at(propertName);
		const JSON& json = children[index];
		if (!json.defined)
			continue;
		if (hasChildren) {
			if (newLines) {
				out << ",\n";
				for (int i = 0; i < tabulation * indent; ++i)
					out << " ";
			}
			else
				out << ", ";
		}
		hasChildren = true;
		switch (json.type) {
		case Primitives::STRING:
			out << '"' + propertName + "\": " + '"' + json.asString(tabulation, indent) + '"';
			break;
		default:
			out << '"' + propertName + "\": " + json.asString(tabulation, indent);
			break;
		}
	}
	if (hasChildren) {
		if (newLines)
			out << "\n";
		for (int i = 0; i < tabulation * (indent - 1); ++i)
			out << " ";
	}
	out << "}";
	return out.str();
}

bool JSON::asBool() const {
	try {
		return stod(self);
	}
	catch (...) {}
	return self != "{}" && self != "[]" && self != "" && self != "undefined" && self != "null" && self != "false";
};

int JSON::asInt() const {
	return stoi(self);
};

double JSON::asDouble() const {
	return stod(self);
};

void JSON::operator=(const std::string& other) {
	*this = JSON(other);
}

void JSON::setType(Primitives type) {
	this->type = type;
}

void JSON::setSelf(std::string str) {
	self = str;
}

void JSON::assertType(Primitives type) const {
	if (!defined)
		throw JSONException("Attempting to access inactive property");
	if (this->type != type)
		throw JSONException("this JSON is not of the right type for this operation");
}

bool JSON::isNumber() const {
	std::string arr = "0123456789.";
	for (const auto& s : self)
		if (arr.find(s) == std::string::npos)
			return false;
	return true;
}

std::string JSON::asString(unsigned tabulation, int indent) const {
	switch (type) {
		case Primitives::ARRAY:
			return arrayAsString(tabulation, indent + 1);
		case Primitives::OBJECT:
			return objectAsString(tabulation, indent + 1);
		case Primitives::STRING:
		case Primitives::NUMBER:
		default:
			return self;
	}
}

void JSON::parse(const std::string& str) {
	index = 1;
	switch (self[0]) {
		case '{':
			setType(Primitives::OBJECT);
			parseJSON();
			break;
		case '[':
			setType(Primitives::ARRAY);
			parseArray();
			break;
		case '\'':
		case '"':
			if (self.length() == 1)
				throw JSONException("Bad string");
			self.erase(0, 1);
			if (self[self.length() - 1] == '\'' || self[self.length() - 1] == '"')
				self.erase(self.length() - 1);
			setSelf(self);
			setType(Primitives::STRING);
			break;
		default:
			if (isNumber())
				setType(Primitives::NUMBER);
			else
				setType(Primitives::STRING);
			break;
	}
}

void JSON::parseJSON() {
	parseSpaces();
	while (index < self.length() - 1) {
		bool inString = false;
		std::stringstream propertyName;
		while (index < self.length()) {
			char c = self[index];
			if (c == '\'' || c == '"')
				if (inString)
					break;
				else
					inString = true;
			else if (inString)
				propertyName << c;
			++index;
		}
		std::string pn = propertyName.str();
		if (!inString || pn.length() == 0)
			throw JSONException(("No property name in json while parsing " + self).c_str());
		inString = false;
		++index;
		bool found = false;
		parseSpaces();
		if (self[index] != ':')
			throw JSONException(("could not find ':' associated with \"" + pn + "\" in " + self).c_str());
		++index;
		parseSpaces();
		found = false;
		char next = self[index];
		std::stringstream buffer;
		std::vector<char> stack;
		switch (next) {
		case '[':
		case '{':
			buffer << parseJSONOrArray();
			break;
		case '\'':
		case '"':
			buffer << "'";
			++index;
			while (index < self.length()) {
				char c = self[index];
				buffer << c;
				if (c == '\'' || c == '"')
					break;
				++index;
			}
			++index;
			break;
		default:
			while (index < self.length()) {
				char c = self[index];
				if (c != ' ' && c != ',' && c != '}')
					buffer << c;
				else
					break;
				++index;
			}
			break;
		}
		if (indices.find(pn) != indices.end())
			throw JSONException("Duplicated property name " + pn);
		children.emplace_back(buffer.str(), pn);
		properties.push_back(pn);
		indices.insert(make_pair(pn, children.size() - 1));
		parseSpaces();
		if (self[index] == '}')
			break;
		else if (self[index] == ',')
			continue;
		else {
			std::cout << self;
			throw JSONException("no stop character found after property: " + pn + " in " + self);
		}
	}
}

void JSON::parseArray() {
	while (index < self.length()) {
		parseSpaces();
		if (self[index] == ']')
			break;
		children.emplace_back(std::move(readElement()));
		parseSpaces();
		if (self[index] != ',')
			break;
		++index;
		parseSpaces();
	}
}

JSON JSON::readElement() {
	if (self[index] == '\'' || self[index] == '"')
		return readString();
	else if (self[index] == '{' || self[index] == '[')
		return readJSONOrArray();
	else
		return readNumber();
}

JSON JSON::readString() {
	std::stringstream buffer;
	buffer << "\"";
	++index;
	while (index < self.length()) {
		char c = self[index];
		if (c == '\'')
			c = self[index] = '"';
		buffer << c;
		++index;
		if (c == '"')
			break;
	}
	return JSON(buffer.str());
}

JSON JSON::readNumber() {
	std::stringstream buffer;
	buffer << "";
	bool inNum = true;
	while (index < self.length()) {
		char c = self[index];
		switch (c) {
			case ',':
			case ' ':
			case ']':
			case '}':
				inNum = false;
		}
		if (!inNum)
			break;
		buffer << c;
		++index;
	}
	return JSON(buffer.str());
}

JSON JSON::readJSONOrArray() {
	return JSON(parseJSONOrArray());
}

std::string JSON::parseJSONOrArray() {
	std::stringstream buffer;
	std::vector<char> stack;
	while (index <= self.length()) {
		char c = self[index];
		++index;
		buffer << c;
		switch (c) {
			case '{':
			case '[':
				stack.push_back(c);
				break;
			case '}':
				if (stack.back() == '{')
					stack.pop_back();
				break;
			case ']':
				if (stack.back() == '[')
					stack.pop_back();
				break;
			case '\'':
			case '"':
				if (stack.back() == '\'' || stack.back() == '"')
					stack.pop_back();
				else
					stack.push_back(c);
				break;
		}
		if (stack.size() == 0)
			break;
	}
	return buffer.str();
}

void JSON::parseSpaces() {
	while (index < self.length()) {
		char c = self[index];
		if (c != ' ' && c != '\n' && c != '\t')
			break;
		++index;
	}
}
