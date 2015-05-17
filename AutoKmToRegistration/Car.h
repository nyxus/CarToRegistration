#pragma once
#include "Logbook.h"
#include "Ride.h"
#include <string>

class Car
{
private:
	Logbook logbook;
	std::string manufacture, model;
	int  odometerValue;
	
public:
	Car(std::string manufacture, std::string model, int odometerValue, Logbook logbook = Logbook());
	~Car();
	void addRide(Ride ride);
	void addLogbook(std::wstring filepath);


};

