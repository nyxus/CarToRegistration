#include "stdafx.h"
#include "Logbook.h"
using namespace std;
using namespace boost::posix_time;

Logbook::Logbook(wstring filepath) :filepath(filepath)
{
	this->tripsListSorted = nullptr;
	this->analiseFile(this->filepath);
}

void Logbook::analiseFile(wstring filepath){
	string line;
	wcout << "Opening file: " << filepath << endl;
	ifstream myfile(filepath);
	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			//cout << line << endl;
			
			/*
			REGEX										DESCRPION (match id)
			([0-9]{10,}) ;								# rideID (1):	
			N ([0-9]{2,}). ([0-9]{2})' ([0-9]{2})\" ;	# Start Latitude(2, 3, 4)
			E ([0-9]{3,}). ([0-9]{2})' ([0-9]{2})\" ;	# Start Longitude (5, 6, 7) 
			([0-9]{2}).([0-9]{2}).([0-9]{4}) ;			# Start date (8, 9, 10)
			([0-9]{2}:[0-9]{2}) ;						# Start time (11)
			([0-9]{8,}) ;								# Start odometer(12)
			N ([0-9]{2,}). ([0-9]{2})' ([0-9]{2})\" ;	# End Latitude(13, 14, 15)
			E ([0-9]{3,}). ([0-9]{2})' ([0-9]{2})\" ;	# End Longitude (16, 17, 18) 
			([0-9]{2}).([0-9]{2}).([0-9]{4}) ;			# End date (19,20, 21)
			([0-9]{2}:[0-9]{2}) ;						# End time (22)
			([0-9]{8,}) ;								# End odometer (23) 
			(0|1)  ;									# Ride type: 1 = private, 2 = business (24)
			\"(.{0,})\"									# Comments (25)
			*/
			regex e("([0-9]{10,}) ; N ([0-9]{2,}). ([0-9]{2})' ([0-9]{2})\" ; E ([0-9]{3,}). ([0-9]{2})' ([0-9]{2})\" ; ([0-9]{2}).([0-9]{2}).([0-9]{4}) ; ([0-9]{2}:[0-9]{2}) ; ([0-9]{8,}) ; N ([0-9]{2,}). ([0-9]{2})' ([0-9]{2})\" ; E ([0-9]{3,}). ([0-9]{2})' ([0-9]{2})\" ; ([0-9]{2}).([0-9]{2}).([0-9]{4}) ; ([0-9]{2}:[0-9]{2}) ; ([0-9]{8,}) ;  (0|1)  ; \"(.{0,})\"");
			smatch m;
			if (regex_search(line, m, e)) {
				
				int startOdometer = stoi(m[12], nullptr);
				// check for duplate 
				if (this->trips.count(startOdometer)){
					// duplate found
					cout << stoi(m[1], nullptr) << ": Droped, duplicate" << endl;
					continue;
				}
				// make new ride and save
				GPSCoordinates startGPS(stoi(m[2], nullptr), stoi(m[3], nullptr), stoi(m[4], nullptr), stoi(m[5], nullptr), stoi(m[6], nullptr), stoi(m[7], nullptr));
				GPSCoordinates endGPS(stoi(m[16], nullptr), stoi(m[17], nullptr), stoi(m[18], nullptr), stoi(m[19], nullptr), stoi(m[20], nullptr), stoi(m[21], nullptr));
				int endOdometer = stoi(m[23], nullptr);
				int rideId = stoi(m[1], nullptr);
				ptime startDateTime(from_iso_string(string(m[10]) + string(m[9]) + string(m[8]) + "T" + string(m[11]).erase(2,1)));
				ptime endDateTime(from_iso_string(string(m[21]) + string(m[20]) + string(m[19]) + "T" + string(m[22]).erase(2,1)));
				TYPE rideType;
				switch (stoi(m[24],nullptr)){
					case 0:
						rideType = PRIVATE;
						break;
					case 1:
						rideType = BUSINESS;
						break;
					default:
						throw invaledRideType();
						return;
				}
				
				Ride newRide(rideId, startDateTime, endDateTime, startGPS, endGPS, endOdometer - startOdometer, startOdometer, rideType, string(string(m[25])));
				this->addRide(newRide);
			}
		}
		myfile.close();
	}else 
		cout << "Unable to open file" << endl;
}

void Logbook::addRide(Ride newRide){

	// check if car has driven
 	if (!newRide.distanceKm){
		cout << newRide.rideId << ": Droped, car didnt drive" << endl;
		return;
	}

	// check for empty logbook
	if (!this->trips.size()){
		this->trips.insert(pair<int, Ride>(newRide.startOdometer, newRide));
		this->tripsListSorted = &this->trips[newRide.startOdometer];
		this->tripsByDate[newRide.start.date().year()][newRide.start.date().month()] = &this->trips[newRide.startOdometer];
		cout << newRide.rideId << ": Added first ride" << endl;
		return;
	}
	//check for unique
	if (!(this->trips.find(newRide.startOdometer) == this->trips.end())){
		// duplate found
		cout << newRide.rideId << ": Droped, duplicate" << endl;
		return;
	}

	Ride *ridePrev = this->findPrevRide(newRide), *rideNext = ridePrev->next;
	// check date: newRide start (date time) past ridePrev end (date time) 
	if (!(ridePrev->end < newRide.start)){
		cout << newRide.rideId << ": Droped, date before previous, prev (end): " << to_simple_string(ridePrev->end) << " cur (Start): " << to_simple_string(newRide.start) << endl;
		return;
	}

	//valid new ride
	this->trips.insert(pair<int, Ride>(newRide.startOdometer, newRide));
	this->trips[newRide.startOdometer].prev = ridePrev;
	this->trips[newRide.startOdometer].next = ridePrev->next;
	ridePrev->next = &this->trips[newRide.startOdometer];
	if (rideNext != nullptr){
		rideNext->prev = &this->trips[newRide.startOdometer];
	}
	this->updateTripByDate(&this->trips[newRide.startOdometer]);
	cout << newRide.rideId << ": Added" << endl;
}

void Logbook::updateTripByDate(Ride *newRide){
	// check if trips by date tree needs to be updated
	if (!(this->tripsByDate[newRide->start.date().year()].count(newRide->start.date().month()))){
		//year or month dont exist: add year and month
		this->tripsByDate[newRide->start.date().year()][newRide->start.date().month()] = newRide;
	}
	return;
}

Ride* Logbook::findPrevRide(Ride k){
	// find same or previous year;
	std::map<int, std::map<int, Ride*>>::iterator yearsCur = this->tripsByDate.begin(), yearsPrev = this->tripsByDate.begin();
	while (yearsCur != tripsByDate.end()){
		if (yearsCur->first == k.start.date().year())
			break;
		yearsPrev = yearsCur;
		next(yearsCur);
	}
	// find same or previous month;
	std::map<int, Ride*>::iterator monthCur = yearsPrev->second.begin(), monthPrev = yearsPrev->second.begin();
	while (monthCur != yearsPrev->second.end()){
		if (monthCur->first == k.start.date().month())
			break;
		monthPrev = monthCur;
		next(monthCur);
	}

	// find previous ride
	Ride *rideCur = monthPrev->second, *ridePrev = monthPrev->second;
	while (rideCur != nullptr){
		if (rideCur->startOdometer >= k.startOdometer)
			return ridePrev;
		ridePrev = rideCur;
		rideCur = rideCur->next;
	}
	return ridePrev;
}

void Logbook::addFile(wstring filepath){
	this->analiseFile(filepath);
}

map<int, Ride> Logbook::getTrips() const{
	return this->trips;
}
