#include "DataCollector.h"



DataCollector::DataCollector()
{
}


DataCollector::~DataCollector()
{
}

void DataCollector::SetAfstandBedieningData(std::string input)
{
	//64,64:64,64
	std::regex rgx("(.*?),(.*?):(.*?),(.*)");
	std::smatch m;
	std::regex_search(input, m, rgx);
	try {
		joy1.first = std::stoi(m[1]);
		joy1.second = std::stoi(m[2]);
		joy2.first = std::stoi(m[3]);
		joy2.second = std::stoi(m[4]);
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

