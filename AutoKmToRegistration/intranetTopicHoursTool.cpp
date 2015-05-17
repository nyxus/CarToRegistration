#include "stdafx.h"
#include "intranetTopicHoursTool.h"


intranetTopicHoursTool::intranetTopicHoursTool(std::string page) :webpage(page, "<INPUT TYPE=\"[A - Z]{1, }\" NAME=\"([a - zA - Z_0 - 9]{1, })\" VALUE=\"([0 - 9]{0, })\"[a-zA-Z0-9 \" = ',;()_]{0,}>")
{
	getDataFormFromPage();
}

void intranetTopicHoursTool::getDataFormFromPage(){
	std::cout << std::endl << std::endl << "START REGEX FOR PAGE" << std::endl;
	try {
		std::regex re("<INPUT (TYPE)=\"([A-Z]+)\"");
		std::sregex_iterator next(rawPage.begin(), rawPage.end(), re);
		std::sregex_iterator end;
		std::smatch sm;
		while (next != end) {
			std::smatch match = *next;
			std::cout << match.str() << "\n" << "    subs: " << match[1].str() << ", " << match[2].str()  <<std::endl;
			next++;
		}
	}
	catch (std::regex_error& e) {
		// Syntax error in the regular expression
	}

}
