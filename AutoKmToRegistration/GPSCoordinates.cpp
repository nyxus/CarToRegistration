#include "stdafx.h"
#include "GPSCoordinates.h"


GPSCoordinates::GPSCoordinates(int laG, int laM, int laS, int loG, int loM, int loS)
{
	Latitude[0] = laG;
	Latitude[1] = laM;
	Latitude[2] = laS;

	Longitude[0] = loG;
	Longitude[1] = loM;
	Longitude[2] = loS;
}


GPSCoordinates::~GPSCoordinates()
{
}
