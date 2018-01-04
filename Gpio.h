#pragma once

#include <fstream>


namespace Gpio
{

	enum class Direction
	{
		In, 
		Out
	};

	enum class State
	{
		Low, 
		High
	};

	class Pin
	{

	public:
		
		Pin(unsigned int pinNb, Direction dir = Direction::In) 
		: pinNumber(pinNb), direction(dir)
		{

			// TODO: Check pinNumber

			std::ofstream gpioExport("/sys/class/gpio/export");
			gpioExport << pinNumber;

			SetDirection(direction);
			
		}

		void SetDirection(Direction dir) 
		{
			direction = dir;
			std::ofstream gpioDirection("/sys/class/gpio/gpio" + std::to_string(pinNumber) + "/direction");

			switch(direction)
			{
				case Direction::In:  gpioDirection << "in"; break;
				case Direction::Out: gpioDirection << "out"; break;
				default: break;
			}
		}

		void SetState(State s)
		{
			state = s;
			std::ofstream gpioPin("/sys/class/gpio/gpio" + std::to_string(pinNumber) + "/value");

			switch(state)
			{
				case State::Low:  gpioPin << 0; break;
				case State::High: gpioPin << 1; break;
				default: break;
			}
		}

	private:

		unsigned int pinNumber;
		Direction direction;
		State state;

	};
}
