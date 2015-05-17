#pragma once
class GPSCoordinates
{

private:
	int Latitude[3], Longitude[3];
public:
	GPSCoordinates(){};
	GPSCoordinates(int laG, int laM, int laS, int loG, int loM, int loS);
	~GPSCoordinates();
	
};

