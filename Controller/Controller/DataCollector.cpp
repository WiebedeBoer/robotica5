#include "DataCollector.h"



DataCollector::DataCollector()
{
}


DataCollector::~DataCollector()
{
}

void DataCollector::SetAfstandBedieningData(std::string input)
{

	std::regex rgx("(.*?);(.*?);(.*?);(.*?);(.*?);(.*)");
	std::smatch m;
	std::regex_search(input, m, rgx);

	try {
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
		std::cout << m[1] << std::endl;
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
		std::cout << modus << std::endl;
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