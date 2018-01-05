
#include "Gpio.h"

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

using namespace std::chrono;
using namespace std::this_thread;

class Line 
{

public:

	friend std::istream&operator>>(std::istream& is, Line& l) 
	{
		std::getline(is, l.data);
		return is;
	}

	operator std::string() const { return data; }    

private:
	
	std::string data;
};



std::map<std::string, unsigned> configMap
{
	{"Refresh_rate", 0}, {"GPIO_pin", 0}, {"Temp_threshold", 0}, {"Delday", 0}
};

int main(int argc, char* argv[])
{

	std::ifstream configFile("RasPiFan.config");

	std::vector<std::string> lines;
	std::copy(std::istream_iterator<Line>(configFile), std::istream_iterator<Line>(), std::back_inserter(lines));

	for(const auto& l : lines)
	{
		auto pos = l.find("=");
		std::string param = l.substr(0, pos);
		param.erase(std::remove(param.begin(), param.end(), ' '), param.end());

		auto itConfig = configMap.find(param);
		if(itConfig != configMap.end())
		{
			std::string value = l.substr(pos + 1);
			value.erase(std::remove(value.begin(), value.end(), ' '), value.end());

			std::istringstream iss;
			iss.str(value);

			iss >> itConfig->second;
		}	
	

		std::cout << param << " = " << itConfig->second << std::endl;
	}

	const unsigned int pinNumber = configMap["GPIO_pin"];
	Gpio::Pin pin(pinNumber, Gpio::Direction::Out);
	pin.SetState(Gpio::State::Low);

	while(1)
	{
		std::ifstream file("/sys/class/thermal/thermal_zone0/temp");


        double temp;
        file >> temp;

		temp /= 1000.;

		if(temp > configMap["Temp_threshold"])
			pin.SetState(Gpio::State::High);
		else
			pin.SetState(Gpio::State::Low);

		std::cout << "Temp : " << temp << std::endl;
		sleep_for(milliseconds(configMap["Refresh_rate"]));
	}

	return 0;
}
