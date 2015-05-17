#pragma once
#include <string>
class Person
{
private:
	std::string name;
	
public:
	Person(std::string name){this->name = name;};
	~Person();
};

