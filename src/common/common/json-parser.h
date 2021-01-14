#include "json.h"

class JSONParser {
	unsigned index;
	std::string self;
	JSON* json;
public:
	JSONParser(JSON* json);
	void parse(std::string str);
	void parseJSON();
	void parseArray();
	JSON readElement();
	JSON readString();
	JSON readNumber();
	JSON readJSONOrArray();
	std::string parseJSONOrArray();
	void parseSpaces();
};
