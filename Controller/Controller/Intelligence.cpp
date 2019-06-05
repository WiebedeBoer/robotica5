#include "Intelligence.h"
#include <sstream>
#include <iostream>
#include <string>

Intelligence::Intelligence(DataCollector* DC, GuardedQueue<Command>* GQ, GuardedQueue<Command>* VQ, bool* RN, MicroController* WKR, MicroController*SNR, Vision* V)
{
	Database = DC;
	CommandQueue = GQ;
	VisionQueue = VQ;
	running = RN;
	Worker = WKR;
	Sensor = SNR;
	VisionApi = V;
}

Intelligence::~Intelligence()
{
}

int RefreshInterfal = 200;
int PrintInterfal = 1000;
int ArmInterfal = 500000000;
int DriveInterfal = 200000000;
int VisionInterfall = 1000;
int GripperInterval = 100000000;

std::chrono::system_clock::time_point refreshAfstandBediening = std::chrono::system_clock::now() + std::chrono::milliseconds(RefreshInterfal);
std::chrono::system_clock::time_point PrintJoystick = std::chrono::system_clock::now() + std::chrono::milliseconds(PrintInterfal);
std::chrono::system_clock::time_point MoveArm = std::chrono::system_clock::now() + std::chrono::milliseconds(ArmInterfal);
std::chrono::system_clock::time_point Drive = std::chrono::system_clock::now() + std::chrono::milliseconds(DriveInterfal);
std::chrono::system_clock::time_point RefreshVision = std::chrono::system_clock::now() + std::chrono::milliseconds(VisionInterfall);
std::chrono::system_clock::time_point GripperVision = std::chrono::system_clock::now() + std::chrono::milliseconds(GripperInterval);

void Intelligence::Think()
{

	while (*running == true) {
		Intelligence::CheckAfstandbediening();
		//Intelligence::CheckVision();
		//Intelligence::CheckDrive();
		Intelligence::CheckArm();
		if (std::chrono::system_clock::now() > PrintJoystick) {
			CommandQueue->push(Command(Sensor, "GetJoystick", Database));
			PrintJoystick = std::chrono::system_clock::now() + std::chrono::milliseconds(PrintInterfal);
		}
		if (std::chrono::system_clock::now() > GripperVision) {
			std::vector<std::string> args;
			//= Database->eggDistance;
			int eggdis = 0;
			//std::cout << Intelligence::Database->wedstrijd.eggDistance << std::endl;
			if (!Intelligence::Database->wedstrijd.eggDistance.empty()) {
				eggdis = std::stoi(Intelligence::Database->wedstrijd.eggDistance);
				if (eggdis != 0 && eggdis != NULL) {
					if (eggdis < 30) {
						//if (std::stoi(Intelligence::Database->wedstrijd.eggDistance) < 30) {
							//CommandQueue->push(Command(Worker, "ArmForward", Database, args));
						CommandQueue->push(Command(Worker, "DriveForward", Database, args));
					}
				}
			}
			GripperVision = std::chrono::system_clock::now() + std::chrono::milliseconds(GripperInterval);
		}
	}
}

void Intelligence::CheckDrive()
{
	if (std::chrono::system_clock::now() > Drive) {

		std::vector<std::string> args;
		args.push_back("");
		std::pair<int, int>* Tempjoy2 = new std::pair<int, int>(Database->GetJoy2());
		if (joy2->second > 35) {//driveleft
			args[0] = "32";
			if (joy2->second > 45) {
				args[0] = "64";
				if (joy2->second > 55) {
					args[0] = "128";
				}
			}
			CommandQueue->push(Command(Worker, "DriveLeft", Database, args));
		}
		if (joy2->second < 30) {//DriveRight
			args[0] = "32";
			if (joy2->second < 20) {
				args[0] = "64";
				if (joy2->second < 10) {
					args[0] = "128";
				}
			}
			CommandQueue->push(Command(Worker, "DriveRight", Database, args));

		}
		if (joy2->first > 30 && joy2->first < 34 && joy2->second > 30 && joy2->second < 34) {//StopDriving
			CommandQueue->push(Command(Worker, "DriveStop", Database, args));
		}
		if (joy2->first > 35) {//DriveForward
			args[0] = "16";
			if (joy2->first > 45) {
				args[0] = "64";
				if (joy2->first > 55) {
					args[0] = "128";
				}
			}
			CommandQueue->push(Command(Worker, "DriveForward", Database, args));

		}
		if (joy2->first < 30) {//DriveBackward
			args[0] = "16";
			if (joy2->first < 20) {
				args[0] = "64";
				if (joy2->first < 10) {
					args[0] = "128";
				}
			}
			CommandQueue->push(Command(Worker, "DriveBackward", Database, args));

		}
		joy2 = Tempjoy2;

		Drive = std::chrono::system_clock::now() + std::chrono::milliseconds(DriveInterfal);
	}
}

void Intelligence::CheckVision()
{
	if (std::chrono::system_clock::now() > RefreshVision) {

		VisionQueue->push(Command(VisionApi, "chickenSurvivalRun", Database));
		VisionQueue->push(Command(VisionApi, "eggDistance", Database));
		VisionQueue->push(Command(VisionApi, "qrDistance", Database));
		RefreshVision = std::chrono::system_clock::now() + std::chrono::milliseconds(VisionInterfall);
	}
}

void Intelligence::CheckAfstandbediening()
{
	if (std::chrono::system_clock::now() > refreshAfstandBediening) {
		std::vector<std::string> args;
		args.push_back(std::to_string(Database->speed));
		CommandQueue->push(Command(Sensor, "refresh", Database, args));
		refreshAfstandBediening = std::chrono::system_clock::now() + std::chrono::milliseconds(RefreshInterfal);
	}
}

void Intelligence::CheckArm()
{
	if (std::chrono::system_clock::now() > MoveArm) {
		std::vector<std::string> args;
		std::pair<int, int>* Tempjoy1 = new std::pair<int, int>(Database->GetJoy1());
		args.push_back(std::to_string(joy1->first));
		args.push_back(std::to_string(joy1->second));


		if (joy1->first > 40) {
			CommandQueue->push(Command(Worker, "ArmLeft", Database, args));
		}
		else if (joy1->first < 20) {
			CommandQueue->push(Command(Worker, "ArmRight", Database, args));
		}
		else if (joy1->second > 40) {
			CommandQueue->push(Command(Worker, "ArmForward", Database, args));
		}
		else if (joy1->second < 20) {
			CommandQueue->push(Command(Worker, "ArmBackward", Database, args));
		}
		joy1 = Tempjoy1;

		MoveArm = std::chrono::system_clock::now() + std::chrono::milliseconds(ArmInterfal);
	}
}

