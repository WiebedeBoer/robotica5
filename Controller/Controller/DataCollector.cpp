#include "DataCollector.h"



DataCollector::DataCollector()
{
}


DataCollector::~DataCollector()
{
}

void DataCollector::SetSensorInfo(std::string input)
{
	sensorInfo = input;
	std::regex rgx("([0-9.]+)");
	std::smatch matches;
	//std::cout << "input regex gino we all gucci yay: " << input << std::endl;

	if (std::regex_search(input, matches, rgx))
	{
		try
		{
			microphone[0] = std::stoi(matches[4]);
			microphone[1] = std::stoi(matches[5]);
			microphone[2] = std::stoi(matches[6]);
		}
		catch (const std::exception &) {
			//std::cout << "sensor stoi error" << std::endl;
		}
	}
	else
	{
		std::cout << "No Sensor values" << std::endl;
	}
}

void DataCollector::SetAfstandBedieningData(std::string input)
{

	//std::regex rgx("(.*?);(.*?);(.*?);(.*?);(.*?);(.*)");
	std::regex rgx("(.*?);(.*?);(.*?);(.*?);(.*?);(.*);(.*)");
	std::smatch m;
	std::regex_search(input, m, rgx);

	try {
		if (m[7] == "1") {
			shutdown = m[7];//shutdown check
			std::cout << "shutdown immiment" << std::endl;
		}

		joy1.first = std::stoi(m[2]);
		joy1.second = std::stoi(m[3]);
		joy2.first = std::stoi(m[4]);
		joy2.second = std::stoi(m[5]);
		if (std::stoi(m[6]) == 1) {
			if (grab == false) {
				updateGrab = true;
				grab = true;
			}
		}
		else
		{
			if (grab == true) {
				updateGrab = true;
				grab = false;
			}
		}
		// alleen kwalificatie modus zijn toegevoegd
		//std::cout << m[1] << std::endl;
		if (m[1] == "Poortje")
			modus = modus::poortje;
		else if (m[1] == "Chicken")
			modus = modus::chickenSurvivalRun;
		else if (m[1] == "Grind")
			modus = modus::grindpad;
		else if (m[1] == "Balkje")
			modus = modus::BlueBeam;
		else if (m[1] == "Start")
			modus = modus::Controller;
		else if (m[1] == "Pitch")
			modus = modus::pitch;
		else if (m[1] == "Arm")
			modus = modus::arm;
		else if (m[1] == "DanceSi")
			modus = modus::DanceSi;
		else if (m[1] == "DanceLi")
			modus = modus::DanceLi;
		//std::cout << modus << std::endl;
	}
	catch (const std::exception &) {
		std::cout << "joy stoi error" << std::endl;
	}


	DataCollector::speed = joy1.first;
}

std::pair<int, int> DataCollector::GetJoy1()
{
	return DataCollector::joy1;
}

std::pair<int, int> DataCollector::GetJoy2()
{
	return DataCollector::joy2;
}