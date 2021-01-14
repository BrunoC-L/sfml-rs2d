#include "pch.h"
#include "json-parser.h"

JSONParser::JSONParser(JSON* json) {
	this->json = json;
}

void JSONParser::parse(std::string str) {
	self = str;
	index = 1;
	switch (self[0]) {
		case '{':
			parseJSON();
			json->setType(Primitives::OBJECT);
			break;
		case '[':
			json->setType(Primitives::ARRAY);
			parseArray();
			break;
		case '\'':
		case '"':
			self.erase(0, 1);
			if (self[self.length() - 1] == '\'' || self[self.length() - 1] == '"')
				self.erase(self.length() - 1);
			json->setSelf(self);
			json->setType(Primitives::STRING);
			break;
		default:
			json->setType(Primitives::NUMBER);
			break;
	}
}

void JSONParser::parseJSON() {
	parseSpaces();
	while (index < self.length() - 1) {
		bool inString = false;
		std::string propertyName = "";
		while (index < self.length()) {
			char c = self[index];
			if (c == '\'' || c == '"')
				if (inString)
					break;
				else
					inString = true;
			else if (inString)
				propertyName += c;
			++index;
		}
		if (!inString || propertyName.length() == 0)
			throw new JSONException(("No property name in json while parsing " + self).c_str());
		inString = false;
		++index;
		bool found = false;
		parseSpaces();
		if (self[index] != ':')
			throw JSONException(("could not find ':' associated with \"" + propertyName + "\" in " + self).c_str());
		++index;
		parseSpaces();
		found = false;
		char next = self[index];
		std::string buffer = "";
		std::vector<char> stack;
		switch (next) {
		case '[':
		case '{':
			buffer = parseJSONOrArray();
			break;
		case '\'':
		case '"':
			buffer = "'";
			++index;
			while (index < self.length()) {
				char c = self[index];
				buffer += c;
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
					buffer += c;
				else
					break;
				++index;
			}
			break;
		}
		json->children.push_back(JSON(buffer, propertyName));
		json->indices.insert(make_pair(propertyName, json->children.size() - 1));
		parseSpaces();
		if (self[index] == '}')
			break;
		else if (self[index] == ',')
			continue;
		else
			throw JSONException(("no stop character found after property: " + propertyName + " in " + self).c_str());
	}
}

void JSONParser::parseArray() {
	while (index < self.length()) {
		parseSpaces();
		if (self[index] == ']')
			break;
		json->children.push_back(readElement());
		parseSpaces();
		if (self[index] != ',')
			break;
		++index;
		parseSpaces();
	}
}

JSON JSONParser::readElement() {
	if (self[index] == '\'' || self[index] == '"')
		return readString();
	else if (self[index] == '{' || self[index] == '[')
		return readJSONOrArray();
	else
		return readNumber();
}

JSON JSONParser::readString() {
	std::string buffer = "\"";
	++index;
	while (index < self.length()) {
		char c = self[index];
		if (c == '\'')
			c = self[index] = '"';
		buffer += c;
		++index;
		if (c == '"')
			break;
	}
	return JSON(buffer);
}

JSON JSONParser::readNumber() {
	std::string buffer = "";
	while (index < self.length()) {
		char c = self[index];
		switch (c) {
		case ',':
		case ' ':
			return JSON(buffer);
		}
		buffer += c;
		++index;
	}
	return JSON(buffer);
}

JSON JSONParser::readJSONOrArray() {
	return JSON(JSON(parseJSONOrArray()));
}

std::string JSONParser::parseJSONOrArray() {
	std::string buffer = "";
	std::vector<char> stack;
	while (index <= self.length()) {
		char c = self[index];
		++index;
		buffer += c;
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
	return buffer;
}

void JSONParser::parseSpaces() {
	while (index < self.length()) {
		if (self[index] != ' ')
			break;
		++index;
	}
}
