#pragma once

#include <algorithm>
#include <fstream>
#include <iterator>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

class Line 
{

public:

	friend std::istream& operator>>(std::istream& is, Line& l) 
	{
		std::getline(is, l.data);
		return is;
	}

	operator std::string() const { return data; }    

private:
	
	std::string data;
};

class Config
{

public:

	Config(const std::string& file) : configFile(file) 
	{
	
		// Parse config file
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
				
				iss >> *itConfig->second;
			}	
		

			std::cout << param << " = " << *itConfig->second << std::endl;
		}
	}

	~Config()
	{
	}

	

private:

	std::ifstream configFile;
	unsigned refreshTime = 0;
	unsigned gpioPin = 0;
	unsigned tempThreshold = 0;
	unsigned delay = 0;

	const std::map<std::string, unsigned*> configMap
	{
		{"Refresh_time", &refreshTime}, {"GPIO_pin", &gpioPin}, {"Temp_threshold", &tempThreshold}, {"Delay", &delay}
	};

};
