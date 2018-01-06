
#include "Gpio.h"
#include "Config.h"

#include <chrono>
#include <fstream>
#include <iostream>
#include <thread>

using namespace std::chrono;
using namespace std::this_thread;




int main(int argc, char* argv[])
{

	Config config("RasPiFan.config");


	const unsigned int pinNumber = 4;
	Gpio::Pin pin(pinNumber, Gpio::Direction::Out);
	pin.SetState(Gpio::State::Low);

	while(1)
	{
		std::ifstream file("/sys/class/thermal/thermal_zone0/temp");


        double temp;
        file >> temp;

		temp /= 1000.;

		if(temp > 60)
			pin.SetState(Gpio::State::High);
		else
			pin.SetState(Gpio::State::Low);

		std::cout << "Temp : " << temp << std::endl;
		sleep_for(milliseconds(2000));
	}

	return 0;
}
