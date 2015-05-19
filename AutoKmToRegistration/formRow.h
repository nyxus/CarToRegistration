#pragma once
#include <string>
class formRow
{
public:
	formRow(std::string name, std::string value, std::string type) :
		name(name), value(value), type(type){
		
	};
	~formRow(){};
	std::string getName(){ return name; };
	std::string getValue(){ return value; };
	std::string getType(){ return type; };
private:
	std::string name, value, type;
};

