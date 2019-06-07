#pragma once
#include<utility>
#include<string>
#include<regex>
#include<iostream>
//DataCollection for the Wedstrijd
struct Wedstrijd
{
	std::string eggDistance;
	std::string chickenSurvivalRun;
	std::string qrDistance;
	std::string qrDestination;
};
//DataCollection for the Qualification
struct Kwalificatie
{
	std::string pitch;
	std::string poort;
	std::string grindpad;
	std::string eiGripper;
	std::string vision; //blue beam
	std::string GripperVision;
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
struct Modus {
	enum mainModus { _Mkwalificatie, _Mwedstrijd } mm;
	enum subQualifyModus { pitch, poortje, grindpad, eiGripper, vision } sqm;
	enum subMatchModus { chickenSurvivalRun, eggtelligence }smm;
	enum eggModus { eggDistance, qrDistance };
	enum townModus { Hertogenbosch, Eindhoven, Eibergen, Barneveld, Duckstad }tm;
};
class DataCollector
{
public:
	DataCollector();
	~DataCollector();
	void SetAfstandBedieningData(std::string input);
	std::pair<int, int> GetJoy1();
	std::pair<int, int> GetJoy2();
	int speed;
	Wedstrijd wedstrijd;
	Kwalificatie kwalificatie;
	Arm arm;
private:
	std::pair<int, int> joy1 = std::make_pair(0, 0);
	std::pair<int, int> joy2 = std::make_pair(0, 0);

	int gyroscoop;
};
