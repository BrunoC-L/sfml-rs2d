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

enum class Primitives { STRING, NUMBER, ARRAY, OBJECT };

class JSON {
private:
	unsigned index;
	std::string self;
	Primitives type;
	bool defined = true;

	std::string arrayAsString(unsigned tabulation, int indent) const;
	std::string objectAsString(unsigned tabulation, int indent) const;

	void parse(const std::string& str);
	void parseJSON();
	void parseArray();
	JSON readElement();
	JSON readString();
	JSON readNumber();
	JSON readJSONOrArray();
	std::string parseJSONOrArray();
	void parseSpaces();
	JSON getInactive(std::string propertyName);
	std::string propertyName;
	std::vector<std::string> properties;
	std::unordered_map<std::string, unsigned> indices;
	void setType(Primitives type);
	void setSelf(std::string str);
	void assertType(Primitives type) const;
	bool isNumber() const;
	std::string asString(unsigned tabulation, int indent) const;
	std::vector<JSON> children;
public:

	JSON();
	JSON(std::string&& json);
	JSON(const std::string& json);
	JSON(const std::string& json, std::string propertyName);
	JSON(JSON&& other);
	JSON(const JSON& other) = default;
	void operator=(const JSON & other);
	void operator=(const std::string & other);

	unsigned size() const;
	const std::vector<std::string>& getProperties() const;
	const std::vector<JSON>& getChildren() const;
	const std::string& getName() const;

	bool has(const std::string& propertyName) const;
	const JSON& get(const std::string& propertyName) const;
	const JSON& get(int x) const;
	JSON& operator[](const std::string& propertyName);
	JSON& operator[](int x);

	void push(JSON&& json);
	void push(const JSON& json);
	void push(std::string str);

	bool asBool() const;
	int asInt() const;
	double asDouble() const;
	std::string asString() const;
	std::string asString(unsigned tabulation) const;
};
