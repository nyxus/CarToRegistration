#pragma once
#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <Windows.h>
#include <regex>
#include "Ride.h"
#include "GPSCoordinates.h"
#include "boost/date_time/posix_time/posix_time.hpp"
#include "invaledRideType.h"

class Logbook
{
private:
	std::map<int, Ride> trips;
	Ride *tripsListSorted;
	std::map<int, std::map<int, Ride*>> tripsByDate;
	void analiseFile(std::wstring filepath);
	std::wstring filepath;
	
	Ride* findPrevRide(Ride k);
	void Logbook::updateTripByDate(Ride *newRide);
public:
	Logbook(){
		trips = std::map<int, Ride>(); filepath = std::wstring(); tripsListSorted = nullptr; 
	};
	Logbook(std::wstring filename);
	void addFile(std::wstring filepath);
	void addRide(Ride newRide);
	void addLogbook(Logbook newLogbook);
	std::map<int, Ride> getTrips() const;
};
