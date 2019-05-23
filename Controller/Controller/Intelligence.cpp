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

	int RefreshInterfal = 50;
	int PrintInterfal = 100000;
	int ArmInterfal = 100;
	int VisionInterfall = 10000;
	std::chrono::system_clock::time_point RefreshController = std::chrono::system_clock::now() + std::chrono::milliseconds(RefreshInterfal);
	std::chrono::system_clock::time_point PrintJoystick = std::chrono::system_clock::now() + std::chrono::milliseconds(PrintInterfal);
	std::chrono::system_clock::time_point MoveArm = std::chrono::system_clock::now() + std::chrono::milliseconds(ArmInterfal);
	std::chrono::system_clock::time_point RefreshVision = std::chrono::system_clock::now() + std::chrono::milliseconds(VisionInterfall);

	while (*running == true) {
		if (std::chrono::system_clock::now() > RefreshController) {
			std::vector<std::string> args;
			args.push_back(std::to_string(Database->speed));
			CommandQueue->push(Command(Sensor, "refresh?", Database, args));
			RefreshController = std::chrono::system_clock::now() + std::chrono::milliseconds(RefreshInterfal);
		}
		if (std::chrono::system_clock::now() > PrintJoystick) {
			CommandQueue->push(Command(Sensor, "GetJoystick", Database));
			PrintJoystick = std::chrono::system_clock::now() + std::chrono::milliseconds(PrintInterfal);
		}
		if (std::chrono::system_clock::now() > RefreshVision) {
			VisionQueue->push(Command(VisionApi, "chickenSurvivalRun",Database));//chickenSurvivalRun	
			VisionQueue->push(Command(VisionApi, "eggDistance", Database));
			VisionQueue->push(Command(VisionApi, "qrDistance", Database));
			VisionQueue->push(Command(VisionApi, "pitch", Database));
			VisionQueue->push(Command(VisionApi, "poort", Database));
			VisionQueue->push(Command(VisionApi, "grindpad", Database));
			VisionQueue->push(Command(VisionApi, "eiGripper", Database));
			VisionQueue->push(Command(VisionApi, "vision", Database));
			RefreshVision = std::chrono::system_clock::now() + std::chrono::milliseconds(VisionInterfall);
		}
		if (std::chrono::system_clock::now() > MoveArm) {
			std::vector<std::string> args;
			std::pair<int, int>* Tempjoy1 = new std::pair<int, int>(Database->GetJoy1());
			std::pair<int, int>* Tempjoy2 = new std::pair<int, int>(Database->GetJoy2());
			args.push_back(std::to_string(joy1->first));
			args.push_back(std::to_string(joy1->second));
			args.push_back(std::to_string(joy2->first));
			args.push_back(std::to_string(joy2->second));

			if(joy1->first > 40)
			CommandQueue->push(Command(Worker, "ArmForward", Database, args));

			if(joy1->first < 20)
  			CommandQueue->push(Command(Worker, "ArmBackward", Database, args));

			joy1 = Tempjoy1;
			joy2 = Tempjoy2;
	
			//CommandQueue->push(Command(Worker, "MoveArm", Database, args));
			MoveArm = std::chrono::system_clock::now() + std::chrono::milliseconds(ArmInterfal);
		}
	}
}



