#pragma once
#include "webpage.h"
class intranetTopicHoursTool :
	public webpage
{
public:
	intranetTopicHoursTool(std::string);
	~intranetTopicHoursTool(){};
private:
	void getDataFormFromPage();

};

