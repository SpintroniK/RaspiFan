
#include "Gpio.h"
#include "Config.h"
#include "Temp.h"

#include <chrono>
#include <fstream>
#include <iostream>
#include <thread>

using namespace std::chrono;
using namespace std::this_thread;
using namespace Gpio;

int main(int argc, char* argv[])
{

	Config config("RasPiFan.config");
	
	const unsigned int pinNumber = config.GetGpioPin();
	const unsigned int dt = config.GetRefreshTime();

	Pin pin(pinNumber, Direction::Out);
	pin.SetState(State::Low);

	Temp temp;

	unsigned int threshTime = 0;

	while(1)
	{

		double tmp = temp.Read();

		if(tmp > config.GetTempThreshold() && IsLow(pin.GetState()))
		{
			threshTime += dt;
		}
		else
		{
			if(IsHigh(pin.GetState()) && tmp < config.GetTempThreshold())
			{
				//std::cout << "Set Low" << std::endl;
				pin.SetState(Gpio::State::Low);
			}
		}

		if(threshTime > config.GetDelay() && IsLow(pin.GetState()))
		{
			//std::cout << "Set High" << std::endl;
			pin.SetState(Gpio::State::High);
			threshTime = 0;
		}

		//std::cout << "Temp : " << tmp << std::endl;

		sleep_for(milliseconds(dt));
	}

	return 0;
}
