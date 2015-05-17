#pragma once
#include <iostream>
#include <string>
#include <map>
#include <regex>
class webpage
{
	
public:
	webpage(std::string, std::string);
	~webpage(){};
	std::map<std::string, std::string> getFormData();
protected:
	std::string rawPage;
	std::string regexForm;
	std::map<std::string, std::string> formData;

	virtual void getDataFormFromPage() = 0;
};
