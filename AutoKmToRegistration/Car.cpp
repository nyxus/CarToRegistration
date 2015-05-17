#include "stdafx.h"
#include "Car.h"

using namespace std;


Car::Car(string manufacture, string model, int odometerValue, Logbook logbook):
	manufacture(manufacture), 
	model(model), 
	odometerValue(odometerValue), 
	logbook(logbook)
{

}


Car::~Car()
{
}

void Car::addLogbook(wstring filepath){
	this->logbook.addFile(filepath);
	
}
