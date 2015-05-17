#include "stdafx.h"
#include "invaledRideType.h"

const char* invaledRideType::what() const throw()
{
	return "Ride type is invalid";
}