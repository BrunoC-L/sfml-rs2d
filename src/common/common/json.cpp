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
	std::swap(self, other.self);
	std::swap(properties, other.properties);
	type = other.type;
	defined = other.defined;
}

void JSON::operator=(const JSON& other) {
	children = other.children;
	self = other.self;
	properties = other.properties;
	type = other.type;
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
	auto it = find(propertyName);
	int index = it - properties.begin();
	if (it == properties.end()) {
		children.emplace_back(std::move(getInactive(propertyName)));
		properties.push_back(propertyName);
	}
	JSON& child = children[index];
	child.propertyName = propertyName;
	return child;
};

const JSON& JSON::get(const std::string& propertyName) const {
	assertType(Primitives::OBJECT);
	auto it = find(propertyName);
	if (it == properties.end())
		throw JSONException("Missing property, can't provide const reference");
	int index = it - properties.begin();
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

std::vector<std::string>::const_iterator JSON::find(const std::string& propertyName) const {
	return std::find(properties.begin(), properties.end(), propertyName);
}

bool JSON::has(const std::string& propertyName) const {
	return find(propertyName) != properties.end();
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

std::string JSON::asString(std::string tabulation, bool replaceEscapeSequences) const {
	return _asString(tabulation, 0, replaceEscapeSequences);
}

std::string JSON::_arrayAsString(std::string tabulation, int indent, bool replaceEscapeSequences) const {
	bool newLines = tabulation.length() > 0;
	std::stringstream out;
	out << "[";
	if (children.size()) {
		if (newLines)
			out << "\n";
		for (unsigned i = 0; i < indent; ++i)
			out << tabulation;
	}
	bool hasChildren = false;
	for (const auto& json : children) {
		if (!json.defined)
			continue;
		if (hasChildren) {
			if (newLines) {
				out << ",\n";
				for (unsigned i = 0; i < indent; ++i)
					out << tabulation;
			}
			else
				out << ", ";
		}
		hasChildren = true;
		switch (json.type) {
		case Primitives::STRING:
			out << '"' + json._asString(tabulation, indent, replaceEscapeSequences) + '"';
			break;
		default:
			out << json._asString(tabulation, indent, replaceEscapeSequences);
			break;
		}
	}
	if (hasChildren) {
		if (newLines)
			out << "\n";
		for (unsigned i = 0; i < indent - 1; ++i)
			out << tabulation;
	}
	out << "]";
	return out.str();
}

std::string JSON::_objectAsString(std::string tabulation, int indent, bool replaceEscapeSequences) const {
	bool newLines = tabulation.length() > 0;
	std::stringstream out;
	out << "{";
	if (children.size()) {
		if (newLines)
			out << "\n";
		for (unsigned i = 0; i < indent; ++i)
			out << tabulation;
	}
	bool hasChildren = false;
	auto beg = properties.begin();
	for (const auto& propertyName : properties) {
		auto index = find(propertyName) - beg;
		const JSON& json = children[index];
		if (!json.defined)
			continue;
		if (hasChildren) {
			if (newLines) {
				out << ",\n";
				for (unsigned i = 0; i < indent; ++i)
					out << tabulation;
			}
			else
				out << ", ";
		}
		hasChildren = true;
		switch (json.type) {
		case Primitives::STRING:
			out << '"' + propertyName + "\": " + '"' + json._asString(tabulation, indent, replaceEscapeSequences) + '"';
			break;
		default:
			out << '"' + propertyName + "\": " + json._asString(tabulation, indent, replaceEscapeSequences);
			break;
		}
	}
	if (hasChildren) {
		if (newLines)
			out << "\n";
		for (unsigned i = 0; i < indent - 1; ++i)
			out << tabulation;
	}
	out << "}";
	return out.str();
}

bool JSON::asBool() const {
	try {
		return stod(self);
	}
	catch (...) {}
	return !(self == "{}" || self == "[]" || self == "" || self == "undefined" || self == "null" || self == "false");
};

int JSON::asInt() const {
	return stoi(self);
};

double JSON::asDouble() const {
	return stod(self);
}
std::string JSON::asString() const {
	return asString("", false);
}
std::string JSON::asString(bool replaceEscapeSequences) const {
	return asString("", replaceEscapeSequences);
}
;

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

std::string JSON::_asString(std::string tabulation, int indent, bool replaceEscapeSequences) const {
	switch (type) {
		case Primitives::ARRAY:
			return _arrayAsString(tabulation, indent + 1, replaceEscapeSequences);
		case Primitives::OBJECT:
			return _objectAsString(tabulation, indent + 1, replaceEscapeSequences);
		case Primitives::STRING:
		case Primitives::NUMBER:
		default:
			return replaceEscapeSequences ? this->replaceEscapeSequences() : self;
	}
}

std::string JSON::replaceEscapeSequences() const {
	std::string s;
	s.reserve(self.length());
	int i = 0;
	while (i < self.length()) {
		char c = self[i++];
		if (c == '\\') {
			if (i == s.length())
				throw JSONException("Invalid escape sequence");
			char escapeSequence = self[i++];
			switch (escapeSequence) {
				case 't':
					c = '\t';
					break;
				case 'n':
					c = '\n';
					break;
				case 'b':
					c = '\b';
					break;
				case 'r':
					c = '\r';
					break;
				case '\\':
					c = '\\';
					break;
				default:
					throw JSONException("Invalid escape sequence");
			}
		}
		s += c;
	}
	return s;
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
		if (find(pn) != properties.end())
			throw JSONException("Duplicated property name " + pn);
		children.emplace_back(buffer.str(), pn);
		properties.push_back(pn);
		parseSpaces();
		if (self[index] == '}')
			break;
		else if (self[index] == ',')
			continue;
		else {
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
		char c = self[index++];
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
