#pragma once
#include "DataCollector.h"
#include "GuardedQueue.h"
#include "MicroController.h"
#include <thread>
#include <chrono>
#include <utility>
class Intelligence
{
public:
	Intelligence(DataCollector* DC, GuardedQueue<Command>* GQ , GuardedQueue<Command>* VQ, bool* RN, MicroController* WKR, MicroController*SNR, Vision* V);
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

	int RefreshInterfal = 1000;
	int PrintInterfal = 100000000;
	int ArmInterfal = 10000000;
	int DriveInterfal = 2000000;
	int VisionInterfall = 10000;

	std::pair<int, int>* joy1 = new std::pair<int, int>(Database->GetJoy1());
	std::pair<int, int>* joy2 = new std::pair<int, int>(Database->GetJoy2());

	std::chrono::system_clock::time_point refreshAfstandBediening = std::chrono::system_clock::now() + std::chrono::milliseconds(RefreshInterfal);
	std::chrono::system_clock::time_point PrintJoystick = std::chrono::system_clock::now() + std::chrono::milliseconds(PrintInterfal);
	std::chrono::system_clock::time_point MoveArm = std::chrono::system_clock::now() + std::chrono::milliseconds(ArmInterfal);
	std::chrono::system_clock::time_point Drive = std::chrono::system_clock::now() + std::chrono::milliseconds(DriveInterfal);
	std::chrono::system_clock::time_point RefreshVision = std::chrono::system_clock::now() + std::chrono::milliseconds(VisionInterfall);

	void CheckDrive();
	void CheckVision();
	void CheckAfstandbediening();
	void CheckArm();
};

