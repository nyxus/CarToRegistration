#include "stdafx.h"
#include "webpage.h"

webpage::webpage(std::string rawPage, std::string regexForm) :rawPage(rawPage), regexForm(regexForm){

}


std::map<std::string, std::string> webpage::getFormData(){
	return formData;
};
