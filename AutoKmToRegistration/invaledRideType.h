#pragma once
#include <iostream>
#include <exception>

class invaledRideType : public std::exception
{
	virtual const char* what() const throw();
};