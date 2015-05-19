#include "stdafx.h"
#include "intranetTopicHoursTool.h"


intranetTopicHoursTool::intranetTopicHoursTool(std::string page) :webpage(page, "<INPUT TYPE=\"([A-Z]+)\" NAME=\"([a-zA-Z_0-9]+)\" VALUE=\"([0-9]{0,15})\"[a-zA-Z0-9 \"= ',;()_]*>")
{
	getDataFormFromPage();
}

void intranetTopicHoursTool::getDataFormFromPage(){
	std::cout << std::endl << std::endl << "START REGEX FOR PAGE" << std::endl;
	try {
		std::regex re(regexForm);
		std::sregex_iterator next(rawPage.begin(), rawPage.end(), re);
		std::sregex_iterator end;
		std::smatch sm;
		while (next != end) {
			std::smatch match = *next;
			std::cout << match.str() << "\n" << "    subs: " << match[1].str() << ", " << match[2].str()  <<std::endl;
			formData.push_back(formRow(match[1].str(), match[2].str(), match[3].str()));
			next++;
		}
	}
	catch (std::regex_error& e) {
		// Syntax error in the regular expression
	}

}
