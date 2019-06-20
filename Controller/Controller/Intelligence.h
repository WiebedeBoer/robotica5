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

	std::pair<int, int>* joy1 = new std::pair<int, int>(Database->GetJoy1());
	std::pair<int, int>* joy2 = new std::pair<int, int>(Database->GetJoy2());

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

};

