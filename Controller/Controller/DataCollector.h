#pragma once
#include<utility>
#include<string>
#include<regex>
#include<iostream>
struct Wedstrijd
{
	std::string eggDistance;
	std::string chickenSurvivalRun;
	std::string qrDistance;
	std::string qrDestination;

};
struct Kwalificatie
{
	std::string pitch;
	std::string poort;
	std::string grindpad;
	std::string eiGripper;
	std::string vision;
	std::string GripperVision;
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

private:
	std::pair<int, int> joy1;
	std::pair<int, int> joy2;
	// main modus for qualification or match, 0 kwalificatie, 1 wedstrijd
	enum mainModus {_Mkwalificatie, _Mwedstrijd}; 
	mainModus mm;
	//sub modus for qualification, 0 pitch, 1 poortje, 2 grindpad, 3 eiGripper, 4 vision
	enum subQualifyModus {pitch, poortje, grindpad, eiGripper, vision}; 
	subQualifyModus sqm;
	//sub modus for qualification or match, 0 chickenSurvivalRun, 1 eggtelligence
	enum subMatchModus {chickenSurvivalRun, eggtelligence };
	subMatchModus smm;
	//modus phase for eggtelligence match, 0 eggDistance, 1 qrDistance
	enum eggModus {eggDistance, qrDistance};
	eggModus em;
	//city name, 0 Hertogenbosch, 1 Eindhoven, 2 Eibergen, 3 Barneveld, 4 Duckstad
	enum townModus {Hertogenbosch, Eindhoven, Eibergen, Barneveld, Duckstad};
	townModus tm;

	int gyroscoop;
};

