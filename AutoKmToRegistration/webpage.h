#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include "formRow.h"

class webpage
{
	
public:
	webpage(std::string, std::string);
	~webpage(){};
	std::vector <formRow> getFormData();
protected:
	std::string rawPage;
	std::string regexForm;
	std::vector <formRow> formData;

	virtual void getDataFormFromPage() = 0;
};
