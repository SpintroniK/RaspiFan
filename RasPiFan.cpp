
#include "Gpio.h"

#include <thread>
#include <chrono>
#include <iostream>

using namespace std::chrono;
using namespace std::this_thread;
using namespace std::chrono_literals;

int main(int argc, char* argv[])
{

	unsigned int pinNumber = 4;
	Gpio::Pin pin(pinNumber, Gpio::Direction::Out);
	pin.SetState(Gpio::State::Low);


	for(int i = 0; i < 1000; i++)
	{
		std::ifstream file("/sys/class/thermal/thermal_zone0/temp");


        double temp;
        file >> temp;

		temp /= 1000.;

		if(temp > 65.)
			pin.SetState(Gpio::State::High);
		else
			pin.SetState(Gpio::State::Low);

		std::cout << i << " : " << temp << std::endl;
		sleep_for(1000ms);
	}

	return 0;
}
