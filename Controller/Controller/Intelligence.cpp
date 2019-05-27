#include "Intelligence.h"

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

void Intelligence::Think()
{
	std::pair<int, int>* joy1 = new std::pair<int, int>(Database->GetJoy1());
	std::pair<int, int>* joy2 = new std::pair<int, int>(Database->GetJoy2());

	int RefreshInterfal = 50000000;
	int PrintInterfal = 100000000;
	int ArmInterfal = 10000000;
	int DriveInterfal = 2000000;
	int VisionInterfall = 10000;
	std::chrono::system_clock::time_point RefreshController = std::chrono::system_clock::now() + std::chrono::milliseconds(RefreshInterfal);
	std::chrono::system_clock::time_point PrintJoystick = std::chrono::system_clock::now() + std::chrono::milliseconds(PrintInterfal);
	std::chrono::system_clock::time_point MoveArm = std::chrono::system_clock::now() + std::chrono::milliseconds(ArmInterfal);
	std::chrono::system_clock::time_point Drive = std::chrono::system_clock::now() + std::chrono::milliseconds(DriveInterfal);
	std::chrono::system_clock::time_point RefreshVision = std::chrono::system_clock::now() + std::chrono::milliseconds(VisionInterfall);

	while (*running == true) {
		if (std::chrono::system_clock::now() > RefreshController) {
			std::vector<std::string> args;
			args.push_back(std::to_string(Database->speed));
			CommandQueue->push(Command(Sensor, "refresh", Database, args));
			RefreshController = std::chrono::system_clock::now() + std::chrono::milliseconds(RefreshInterfal);
		}
		if (std::chrono::system_clock::now() > PrintJoystick) {
			CommandQueue->push(Command(Sensor, "GetJoystick", Database));
			PrintJoystick = std::chrono::system_clock::now() + std::chrono::milliseconds(PrintInterfal);
		}
		if (std::chrono::system_clock::now() > RefreshVision) {
			VisionQueue->push(Command(VisionApi, "chickenSurvivalRun", Database));
			VisionQueue->push(Command(VisionApi, "eggDistance", Database));
			VisionQueue->push(Command(VisionApi, "qrDistance", Database));
			RefreshVision = std::chrono::system_clock::now() + std::chrono::milliseconds(VisionInterfall);
		}
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
			if (std::chrono::system_clock::now() > MoveArm) {
				std::vector<std::string> args;
				std::pair<int, int>* Tempjoy1 = new std::pair<int, int>(Database->GetJoy1());
				args.push_back(std::to_string(joy1->first));
				args.push_back(std::to_string(joy1->second));


				if (joy1->first > 40)
					CommandQueue->push(Command(Worker, "ArmForward", Database, args));

				if (joy1->first < 20)
					CommandQueue->push(Command(Worker, "ArmBackward", Database, args));

				joy1 = Tempjoy1;

				MoveArm = std::chrono::system_clock::now() + std::chrono::milliseconds(ArmInterfal);
			}
		}
	}



