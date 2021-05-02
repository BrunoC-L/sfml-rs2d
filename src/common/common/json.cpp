#include "pch.h"
#include "json.h"
#include "json-parser.h"

JSON::JSON(std::string json) {
	self = json;
	JSONParser(*this).parse(self);
}

JSON::JSON() {
	self = "{}";
	JSONParser(*this).parse(self);
}

JSON::JSON(std::string json, std::string propertyName) {
	this->propertyName = propertyName;
	self = json;
	JSONParser(*this).parse(self);
}

void JSON::operator=(JSON other) {
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
	return json;
}

JSON& JSON::operator[](std::string propertyName) {
	if (!defined)
		throw JSONException("Attempting to access inactive property");
	if (type != OBJECT)
		throw JSONException("this JSON is either an array, string or number");
	try {
		auto index = indices.at(propertyName);
		return children[index];
	}
	catch (...) {
		children.push_back(getInactive(propertyName));
		indices.insert(make_pair(propertyName, children.size() - 1));
		return children[indices.at(propertyName)];
	}
};

const JSON& JSON::get(std::string propertyName) const {
	if (!defined)
		throw JSONException("Attempting to access inactive property");
	if (type != OBJECT)
		throw JSONException("this JSON is either an array, string or number");
	auto index = indices.at(propertyName);
	return children[index];
}

JSON& JSON::operator[](int x) {
	if (!defined)
		throw JSONException("Attempting to access inactive property");
	if (type != ARRAY)
		throw JSONException("this JSON is not an array");
	return children[x];
}

const JSON& JSON::get(int x) const {
	if (!defined)
		throw JSONException("Attempting to access inactive property");
	if (type != ARRAY)
		throw JSONException("this JSON is not an array");
	return children[x];
}

void JSON::push(JSON json) {
	if (type != ARRAY)
		throw JSONException("this JSON object is not an array");
	this->children.push_back(json);
}

void JSON::push(std::string str) {
	push(JSON(str));
}

std::string JSON::asString() const {
	switch (type) {
		case Primitives::ARRAY:
			return arrayAsString();
		case Primitives::OBJECT:
			return objectAsString();
		case Primitives::STRING:
		case Primitives::NUMBER:
		default:
			return self;
	}
}

std::string JSON::arrayAsString() const {
	std::string out = "[";
	bool hasChildren = false;
	for (const auto& json : children) {
		if (!json.defined)
			continue;
		hasChildren = true;
		switch (json.type) {
			case Primitives::STRING:
				out += '"' + json.asString() + '"';
				break;
			default:
				out += json.asString();
				break;
		}
		out += ", ";
	}
	if (hasChildren)
		out.erase(out.length() - 2, out.length());
	out += ']';
	return out;
}

std::string JSON::objectAsString() const {
	std::string out = "{";
	bool hasChildren = false;
	for (const auto& pair : indices) {
		JSON json = children[pair.second];
		if (!json.defined)
			continue;
		hasChildren = true;
		switch (json.type) {
		case Primitives::STRING:
			out += '"' + pair.first + "\" : " + '"' + json.asString() + '"';
			break;
		default:
			out += '"' + pair.first + "\" : " + json.asString();
			break;
		}
		out += ", ";
	}
	if (hasChildren)
		out.erase(out.length() - 2, out.length());
	out += '}';
	return out;
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

void JSON::operator=(std::string other) {
	*this = JSON(other);
}

void JSON::operator=(std::vector<JSON> v) {
	*this = "[]";
	children = v;
}

void JSON::operator=(std::vector<std::string> v) {
	*this = "[]";
	children.clear();
	for (auto s : v)
		children.push_back(JSON(s));
}

void JSON::setType(Primitives type) {
	this->type = type;
}

void JSON::setSelf(std::string str) {
	self = str;
}
