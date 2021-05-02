#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

class JSONException : public std::exception {
public:
	JSONException(std::string msg) : std::exception(msg.c_str()) {}
	JSONException() : std::exception() {}
};

enum Primitives { STRING, NUMBER, ARRAY, OBJECT };

class JSON {
private:
	std::string self;
	Primitives type;
	bool defined = true;
public:
	JSON getInactive(std::string propertyName);
	std::string propertyName;
	std::vector<JSON> children;
	std::unordered_map<std::string, unsigned> indices;
	void setType(Primitives type);
	void setSelf(std::string str);

	JSON();
	JSON(std::string json);
	JSON(std::string json, std::string propertyName);

	void operator=(JSON other);
	void operator=(std::string other);
	void operator=(std::vector<JSON> v);
	void operator=(std::vector<std::string> v);

	JSON& operator[](std::string propertyName);
	JSON& operator[](int x);

	const JSON& get(std::string propertyName) const;
	const JSON& get(int x) const;

	void push(JSON json);
	void push(std::string str);

	bool asBool() const;
	int asInt() const;
	double asDouble() const;
	std::string asString() const;
	std::string arrayAsString() const;
	std::string objectAsString() const;
};
