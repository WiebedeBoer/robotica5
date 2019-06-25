#pragma once
#include<utility>
#include<string>
#include<regex>
#include<iostream>
//namespace for modus enum
namespace modus {
	enum Modus { BlueBeam, poortje, grindpad, chickenSurvivalRun, eggtelligence, Controller, pitch, arm, DanceSi};
}
//DataCollection for the Wedstrijd
struct Wedstrijd
{
	std::string eggDistance;
	std::string chickenSurvivalRun;
	std::string qrDistance;
	std::string qrDestination;
	std::string findChickin;
};
//DataCollection for the Qualification
struct Kwalificatie
{
	std::string pitch;
	std::string poort;
	std::string grindpad;
	std::string eggLocation;
	std::string bluebeam;
};
//DataCollection for the Arm
struct Arm
{
	int servo1 = 0;
	int servo2 = 0;
	int servo3 = 0;
	int servo4 = 0;
	int servo5 = 0;
};
//DataCollection for the different modus

class DataCollector
{
public:
	DataCollector();
	~DataCollector();
	void SetAfstandBedieningData(std::string input);
	void SetSensorInfo(std::string input);
	std::pair<int, int> GetJoy1();
	std::pair<int, int> GetJoy2();
	
	std::pair<int, int> joy1;
	std::pair<int, int> joy2;
	std::string sensorInfo;
	modus::Modus modus = modus::Controller;
	int speed;
	float horizontalBlueBeam = std::numeric_limits<float>::max();
	Wedstrijd wedstrijd;
	Kwalificatie kwalificatie;
	Arm arm;
	bool updateGrab = false;
	bool grab = false;

private:
	int gyroscoop;
};

