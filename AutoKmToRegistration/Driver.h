#pragma once
#include "Person.h"
#include "Car.h"
#include <string>
#include <vector>

class Driver :
	public Person
{
private:
	std::vector<Car> Cars;
public:
	Driver(std::string name);
	~Driver();
};

