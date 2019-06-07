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
int GripperInterval = 5000;

std::chrono::system_clock::time_point refreshAfstandBediening = std::chrono::system_clock::now() + std::chrono::milliseconds(RefreshInterfal);
std::chrono::system_clock::time_point PrintJoystick = std::chrono::system_clock::now() + std::chrono::milliseconds(PrintInterfal);
std::chrono::system_clock::time_point MoveArm = std::chrono::system_clock::now() + std::chrono::milliseconds(ArmInterfal);
std::chrono::system_clock::time_point Drive = std::chrono::system_clock::now() + std::chrono::milliseconds(DriveInterfal);
std::chrono::system_clock::time_point RefreshVision = std::chrono::system_clock::now() + std::chrono::milliseconds(VisionInterfall);
std::chrono::system_clock::time_point GripperVision = std::chrono::system_clock::now() + std::chrono::milliseconds(GripperInterval);

void Intelligence::Think()
{

	while (*running == true) {
		//Intelligence::CheckAfstandbediening();
		Intelligence::CheckVision();
		Intelligence::CheckDrive();
		Intelligence::CheckArm();
		if (std::chrono::system_clock::now() > PrintJoystick) {
			CommandQueue->push(Command(Sensor, "GetJoystick", Database));
			PrintJoystick = std::chrono::system_clock::now() + std::chrono::milliseconds(PrintInterfal);
		}
		//autonomous qualification
		if (std::chrono::system_clock::now() > GripperVision) {
			//check qualification modus
			std::string QualifyModus;
			//QualifyModus = Intelligence::Database->Modus.subQualifyModus;
			QualifyModus = "eiGripper"; //Let op. Even voor de test.
			//egg qualification
			if (QualifyModus == "eiGripper") {				
				CheckEgg();
			}
			//blue beam qualification
			else if (QualifyModus == "vision") {
				CheckBlueBeam();
			}
			//gripper qualification
			else if (QualifyModus == "GripperVision") {
				CheckGripper();
			}
			GripperVision = std::chrono::system_clock::now() + std::chrono::milliseconds(GripperInterval);
		}
	}
}

//gripper qualification
void Intelligence::CheckGripper() {
	std::vector<std::string> args;
	args.push_back("");
	args[0] = "32";
	args[1] = "36";	
	CommandQueue->push(Command(Worker, "ArmForward", Database, args)); //move arm forward
}

//egg vision qualification
void Intelligence::CheckEgg() {
	std::vector<std::string> args;
	args.push_back("");
	int eggdistance = 0; //distance
	//std::cout << Intelligence::Database->kwalificatie.eiGripper << std::endl;
	if (!Intelligence::Database->kwalificatie.eiGripper.empty()) {
		try {
			eggdistance = std::stoi(Intelligence::Database->kwalificatie.eiGripper);
		}
		catch (int e) {
			std::cout << "stoi error occurred. Exception" << e << '\n';
			eggdistance = 999;
		}
		//if egg seen
		if (eggdistance != 0 && eggdistance != NULL && eggdistance !=999) {
			//to near for gripper, drive backward
			if (eggdistance < 210) {
				args[0] = "32";
				CommandQueue->push(Command(Worker, "DriveBackward", Database, args));
			}
			else if (eggdistance >=210) {
				args[0] = "32";
				CommandQueue->push(Command(Worker, "DriveForward", Database, args));
			}
		}
		//else egg not seen, go seek egg
		else {
			args[0] = "32";
			CommandQueue->push(Command(Worker, "DriveLeft", Database, args)); //search egg in left circle
		}
	}
}

//blue beam vision qualification
void Intelligence::CheckBlueBeam() {
	std::vector<std::string> args;
	args.push_back("");
	int bluedistance = 0; //distance
	int horizontal = 320; //horizontal coordinate
	if (!Intelligence::Database->kwalificatie.vision.empty()) {
		try {
			bluedistance = std::stoi(Intelligence::Database->kwalificatie.vision);
		}
		catch (int e) {
			std::cout << "stoi error occurred. Exception" << e << '\n';
			bluedistance = 999;
		}

		if (bluedistance != 0 && bluedistance != NULL) {
			//if near and in sight, drive
			if (bluedistance >= 5 && bluedistance < 210) {
				//left
				if (horizontal < -10) {
					args[0] = "32";
					CommandQueue->push(Command(Worker, "DriveLeft", Database, args));
				}
				//right
				else if (horizontal > 10) {
					args[0] = "32";
					CommandQueue->push(Command(Worker, "DriveRight", Database, args));
				}
				//forward
				else {
					args[0] = "32";
					CommandQueue->push(Command(Worker, "DriveForward", Database, args));
				}
			}
			//else if too near, full stop
			else if (bluedistance < 5) {
				args[0] = "0";
				CommandQueue->push(Command(Worker, "DriveStop", Database, args));
			}
			/*
			//else if not seen, seek to the left, not sure if necessary
			else if (bluedistance == 999) {
				args[0] = "32";
				CommandQueue->push(Command(Worker, "DriveLeft", Database, args)); //search beam
			}
			*/
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
		VisionQueue->push(Command(VisionApi, "BlueBeam", Database)); //blue beam qualification
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

