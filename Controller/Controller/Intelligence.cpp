#include "Intelligence.h"

Intelligence::Intelligence(DataCollector * DC, GuardedQueue<Command>* GQ, bool* RN, MicroController* WKR, MicroController*SNR)
{
	Database = DC;
	CommandQueue = GQ;
	running = RN;
	Worker = WKR;
	Sensor = SNR;
	
}

Intelligence::~Intelligence()
{
}

void Intelligence::Think()
{
	std::pair<int, int>* joy1 = new std::pair<int, int>(Database->GetJoy1());
	std::pair<int, int>* joy2 = new std::pair<int, int>(Database->GetJoy2());

	int RefreshInterfal = 50;
	int PrintInterfal = 100000;
	int ArmInterfal = 100;
	int AfstandInterval = 100;
	std::chrono::system_clock::time_point RefreshController = std::chrono::system_clock::now() + std::chrono::milliseconds(RefreshInterfal);
	std::chrono::system_clock::time_point PrintJoystick = std::chrono::system_clock::now() + std::chrono::milliseconds(PrintInterfal);
	std::chrono::system_clock::time_point MoveArm = std::chrono::system_clock::now() + std::chrono::milliseconds(ArmInterfal);
	std::chrono::system_clock::time_point AfstandSensor = std::chrono::system_clock::now() + std::chrono::milliseconds(AfstandInterval);

	while (*running == true) {
		if (std::chrono::system_clock::now() > RefreshController) {
			std::vector<std::string> args;
			args.push_back(std::to_string(Database->getspeed()));
			CommandQueue->push(Command(Sensor, "refresh?", Database, args));
			RefreshController = std::chrono::system_clock::now() + std::chrono::milliseconds(RefreshInterfal);
		}
		if (std::chrono::system_clock::now() > PrintJoystick) {
			CommandQueue->push(Command(Sensor, "GetJoystick", Database));
			PrintJoystick = std::chrono::system_clock::now() + std::chrono::milliseconds(PrintInterfal);
		}
		if (std::chrono::system_clock::now() > MoveArm) {
			std::vector<std::string> args;

			std::pair<int, int>* Tempjoy1 = new std::pair<int, int>(Database->GetJoy1());
			std::pair<int, int>* Tempjoy2 = new std::pair<int, int>(Database->GetJoy2());
			if (*joy1 != *Tempjoy1 || *joy2 != *Tempjoy2) {

				joy1 = Tempjoy1;
				joy2 = Tempjoy2;
				args.push_back(std::to_string(joy1->first));
				args.push_back(std::to_string(joy1->second));
				args.push_back(std::to_string(joy2->first));
				args.push_back(std::to_string(joy2->second));
				CommandQueue->push(Command(Worker, "MoveArm", Database, args));
			}
			else {
			}
			MoveArm = std::chrono::system_clock::now() + std::chrono::milliseconds(ArmInterfal);
		}
		if (std::chrono::system_clock::now() > AfstandSensor) {
			std::vector<std::string> args;
			CommandQueue->push(Command(Worker, "Motor", Database, args));
		}
	}
}



