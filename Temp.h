#pragma once

#include <fstream>

class Temp
{

public:

	Temp()
	{
	}

	~Temp()
	{
	}

	double Read() const
	{
	
		std::ifstream file("/sys/class/thermal/thermal_zone0/temp");

		double temp;
        file >> temp;

		return temp / 1000.;
	}

};
