#pragma once
#include <string>
#include "boost/date_time/posix_time/posix_time.hpp"
#include "GPSCoordinates.h"
typedef enum TYPE{ PRIVATE, BUSINESS };

class Ride
{
private:
	enum TYPE type;
	std::string comments;
	GPSCoordinates startGPS, endGPS;

public:
	boost::posix_time::ptime const start, end;
	int const rideId, distanceKm, startOdometer;
	Ride *next, *prev;
	Ride():
		next(nullptr),
		prev(nullptr),
		start(boost::posix_time::ptime()),
		end(boost::posix_time::ptime()),
		rideId(0),
		distanceKm(0),
		startOdometer(0)
		{};
	Ride(const int rideId, boost::posix_time::ptime start, boost::posix_time::ptime end, const GPSCoordinates startGPS, const GPSCoordinates endGPS, int const distanceKm, int const startOdometer, enum TYPE type, std::string comment = "") :
		rideId(rideId),
		start(start),
		end(end),
		startGPS(startGPS),
		endGPS(endGPS),
		distanceKm(distanceKm),
		startOdometer(startOdometer),
		type(type),
		comments(comments),
		next(nullptr),
		prev(nullptr) {};
	~Ride();
};

