#pragma once
#include<utility>
#include<string>
#include<regex>
class DataCollector
{
public:
	DataCollector();
	~DataCollector();
	void SetAfstandBedieningData(std::string input);
	std::pair<int, int> GetJoy1();
	std::pair<int, int> GetJoy2();
	int getspeed();
private:
	std::pair<int, int> joy1;
	std::pair<int, int> joy2;
	int speed;
	int gyroscoop;


};

