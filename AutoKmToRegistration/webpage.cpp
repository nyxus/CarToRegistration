#include "stdafx.h"
#include "webpage.h"

webpage::webpage(std::string rawPage, std::string regexForm) :rawPage(rawPage), regexForm(regexForm){

}


std::vector<formRow> webpage::getFormData(){
	return formData;
};
