#pragma once
#include "DataCollector.h"
#include "GuardedQueue.h"
#include "MicroController.h"
#include <thread>
#include <chrono>
#include <utility>

//this class get data based on time,  on that data it makes new commands to execute
class Intelligence
{
public:
	Intelligence(DataCollector* DC, GuardedQueue<Command>* GQ, GuardedQueue<Command>* VQ, bool* RN, MicroController* WKR, MicroController*SNR, Vision* V);
	~Intelligence();
	void Think();

private:
	DataCollector* Database;
	GuardedQueue<Command>* CommandQueue;
	GuardedQueue<Command>* VisionQueue;
	bool* running;
	MicroController* Worker;
	MicroController* Sensor;
	Vision* VisionApi;
	Command RepeatedCommand;
	std::string armGrip;


	void ExecuteDrive();
	void CheckVision();
	void ExecuteVision();
	void CheckAfstandbediening();
	void ExecuteArm();
	void SplitOn(std::string const &input, char sep, std::vector<std::string>& output);
	void ExecuteBlueBeam();
	void ExecuteEgg();
	void ExecuteChickinSurivalRun();
	void ExecuteSpeak();
	void ExecuteLed();
	void ExecuteDanceSi();

	void CheckInfo();
	void ExecuteDanceLi();
	//eggtelligence sub steps
	std::string CityEggtelligence();
	bool DriveEggtelligence(); //finding egg
	bool ArmEggtelligence(std::string armGrip); //pick up and drop off
	bool TrayEggtelligence(); //finding tray
	void TapeHelper(); //for the tape
	void ChickenHelper(); //for the chicken
};

