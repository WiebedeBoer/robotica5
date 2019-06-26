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

//intervals for when some functions need to happen
int LedInterval = 10000;
int RefreshInterval = 150;
int PrintInterval = 5000;
int ArmInterval = 200;
int DriveInterval = 200;
int CheckVisionInterval = 1000;
int ExecuteVisionInterval = 200;
int GripperInterval = 500;
int SpeakInterval = 3000000;
int RepeatInterval = 500;
int infoInterval = 500;
//corrisponding timers for the intervals
std::chrono::system_clock::time_point refreshAfstandBedieningTime = std::chrono::system_clock::now() + std::chrono::milliseconds(RefreshInterval);
std::chrono::system_clock::time_point PrintJoystickTime = std::chrono::system_clock::now() + std::chrono::milliseconds(PrintInterval);
std::chrono::system_clock::time_point MoveArmTime = std::chrono::system_clock::now() + std::chrono::milliseconds(ArmInterval);
std::chrono::system_clock::time_point DriveTime = std::chrono::system_clock::now() + std::chrono::milliseconds(DriveInterval);
std::chrono::system_clock::time_point RefreshVisionTime = std::chrono::system_clock::now() + std::chrono::milliseconds(CheckVisionInterval);
std::chrono::system_clock::time_point ExecuteVisionTime = std::chrono::system_clock::now() + std::chrono::milliseconds(ExecuteVisionInterval);
std::chrono::system_clock::time_point SpeakTime = std::chrono::system_clock::now() + std::chrono::milliseconds(SpeakInterval);
std::chrono::system_clock::time_point RepeatTime = std::chrono::system_clock::now() + std::chrono::milliseconds(RepeatInterval);
std::chrono::system_clock::time_point LedTime = std::chrono::system_clock::now() + std::chrono::milliseconds(LedInterval);
std::chrono::system_clock::time_point infoTime = std::chrono::system_clock::now() + std::chrono::milliseconds(infoInterval);


void Intelligence::Think()
{
	while (*running == true) {
		Intelligence::CheckAfstandbediening();
		Intelligence::CheckVision();
		Intelligence::ExecuteDrive();
		Intelligence::ExecuteLed();
		Intelligence::ExecuteArm();
		Intelligence::ExecuteVision();
		Intelligence::ExecuteSpeak();		//debug print joystick values
		if (std::chrono::system_clock::now() > PrintJoystickTime) {
			CommandQueue->push(Command(Sensor, "GetJoystick", Database));
			PrintJoystickTime = std::chrono::system_clock::now() + std::chrono::milliseconds(PrintInterval);
		}
	}
}


void Intelligence::ExecuteChickinSurivalRun()
{

}

void Intelligence::ExecuteSpeak()
{
	if (std::chrono::system_clock::now() > SpeakTime) {

		if (Database->modus = modus::pitch) {
			CommandQueue->push(Command(Worker, "speak_pitch", Database));

		}
		else
		{
			CommandQueue->push(Command(Worker, "speak_random", Database));

		}

		SpeakTime = std::chrono::system_clock::now() + std::chrono::milliseconds(SpeakInterval + 999999);
	}
}

void Intelligence::ExecuteEgg() {
	std::vector<std::string> args;
	args.push_back("");
	int distance = 999; //distance
	int horizontal = 0; //horizontal coordinate
	if (!Intelligence::Database->kwalificatie.eggLocation.empty()) {
		std::string s = Intelligence::Database->kwalificatie.eggLocation;
		std::vector<std::string> out;

		SplitOn(ref(s), ':', ref(out));

		try {
			if (out[0] != "False") {
				distance = std::stoi(out[0]);
				horizontal = std::stoi(out[1]);
			}
			else {
				distance = 0;
			}
		}
		catch (int e) {
			std::cout << "stoi distance error occurred. Exception" << e << '\n';
		}


		if (distance != 0 && distance != NULL) {
			//if near and in sight, drive
			if (distance >= 5 && distance < 210) {
				//left
				if (horizontal < -150) {
					args[0] = "64";
					CommandQueue->push(Command(Worker, "DriveLeft", Database, args));
				}
				//right
				else if (horizontal > 150) {
					args[0] = "64";
					CommandQueue->push(Command(Worker, "DriveRight", Database, args));
				}
				//forward
				else {
					args[0] = "64";
					CommandQueue->push(Command(Worker, "DriveBackward", Database, args));
				}
			}

			//else if too near, full stop
			else if (distance < 5) {
				args[0] = "0";
				std::cout << "EggDistanceIsToSmall" << std::endl;
				CommandQueue->push(Command(Worker, "DriveStop", Database, args));
			}
		}
	}
}

//blue beam vision qualification
void Intelligence::ExecuteBlueBeam() {
	std::vector<std::string> args;
	args.push_back("");
	int distance = 999; //distance

	if (!Intelligence::Database->kwalificatie.bluebeam.empty()) {
		std::string s = Intelligence::Database->kwalificatie.bluebeam;
		std::vector<std::string> out;

		SplitOn(ref(s), ':', ref(out));
		float horizontalBlueBeam = std::numeric_limits<float>::max();

		try {
			if (out[0] != "False") {
				distance = std::stoi(out[0]);

				// Write if not initialized
				if (horizontalBlueBeam == std::numeric_limits<float>::max())
					horizontalBlueBeam = std::stof(out[1]);

				// Write if new value
				if (abs(horizontalBlueBeam - 10) > abs(std::stof(out[1])) || abs(horizontalBlueBeam + 10) < abs(std::stof(out[1])))
					horizontalBlueBeam = std::stof(out[1]);
			}
		}
		catch (float e) {
			std::cout << "Stof error occurred. Exception" << e << '\n';
		}

		if (horizontalBlueBeam != std::numeric_limits<float>::max()) {
			//if near and in sight, drive
			if (distance >= 5 && distance < 210) {
				auto argsChange = [](std::string speed, std::vector<std::string> args) { args[0] = speed; return args; };

				//left
				if (horizontalBlueBeam < -50)
					CommandQueue->push(Command(Worker, "DriveLeft", Database, argsChange("128", args)));

				//right
				else if (horizontalBlueBeam > 50)
					CommandQueue->push(Command(Worker, "DriveRight", Database, argsChange("128", args)));
			}

			//else if too near, full stop
			else if (distance < 5) {
				args[0] = "0";
				std::cout << "BlueDistanceIsToSmall" << std::endl;
				CommandQueue->push(Command(Worker, "DriveStop", Database, args));
			}
		}
	}
}


void Intelligence::ExecuteDrive()
{

	std::string slowSpeed = "175";
	std::string midSpeed = "175";
	std::string highSpeed = "175";


	std::string HslowSpeed = "36";
	std::string HmidSpeed = "64";
	std::string HhighSpeed = "64";


	int Pjoylow = 30;
	int Pjoymid = 35;
	int Pjoyhigh = 40;


	int Njoylow = 26;
	int Njoymid = 22;
	int Njoyhigh = 20;

	

	if (std::chrono::system_clock::now() > DriveTime && Database->modus != modus::arm) {
		DriveTime = std::chrono::system_clock::now() + std::chrono::milliseconds(DriveInterval);

			std::vector<std::string> args;
			args.push_back("");
			std::pair<int, int>* Tempjoy2 = new std::pair<int, int>(Database->GetJoy2());

			if (Database->joy2.second > Pjoylow) {//DriveForward
				args[0] = slowSpeed;
				if (Database->joy2.second > Pjoymid) {
					args[0] = midSpeed;
					if (Database->joy2.second > Pjoyhigh) {
						args[0] = highSpeed;
					}
				}
				CommandQueue->push(Command(Worker, "DriveForward", Database, args));

				return;

			}
			if (Database->joy2.second < Njoylow) {//DriveBackward
				args[0] = slowSpeed;
				if (Database->joy2.second < Njoymid) {
					args[0] = midSpeed;
					if (Database->joy2.second < Njoyhigh) {
						args[0] = highSpeed;
					}
				}
				CommandQueue->push(Command(Worker, "DriveBackward", Database, args));

				return;

			}

			if (Database->joy2.first > Pjoylow) {//DriveRight
				args[0] = HslowSpeed;
				if (Database->joy2.first > Pjoymid) {
					args[0] = HmidSpeed;
					if (Database->joy2.first > Pjoyhigh) {
						args[0] = HhighSpeed;
					}
				}
				CommandQueue->push(Command(Worker, "DriveRight", Database, args));

				return;
			}
			if (Database->joy2.first < Njoylow) {//DriveLeft
				args[0] = HslowSpeed;
				if (Database->joy2.first < Njoymid) {
					args[0] = HmidSpeed;
					if (Database->joy2.first < Njoyhigh) {
						args[0] = HhighSpeed;
					}
				}
				CommandQueue->push(Command(Worker, "DriveLeft", Database, args));

				return;

			}
			if (Database->GetJoy2().first > 28 && Database->GetJoy2().first < 32 && Database->GetJoy2().second > 28 && Database->GetJoy2().second < 32) {//StopDriving
				CommandQueue->push(Command(Worker, "DriveStop", Database, args));
				return;
			}
			joy2 = Tempjoy2;
		
	}
}
int dancemove = 0;
void Intelligence::ExecuteDanceLi()
{
	std::vector<std::string> args;
	args.push_back("");
	int activationThreshold = 512;
	if (Database->microphone[0] > activationThreshold)
	{
		switch (dancemove)
		{
		case 1:
			args[0] = "100";
			CommandQueue->push(Command(Worker, "DriveForward", Database, args));
			CommandQueue->push(Command(Worker, "DriveBackward", Database, args));
			CommandQueue->push(Command(Worker, "DriveBackward", Database, args));

			break;
		case 2:
			args[0] = "100";
			break;
		case 3:
			break;
		case 4:
			break;
		default:
			break;
		}
	}
	if (Database->microphone[1] > activationThreshold)
	{

	}
	if (Database->microphone[2] > activationThreshold)
	{

	}
}

void Intelligence::CheckVision()
{
	if (std::chrono::system_clock::now() > RefreshVisionTime) {
		VisionQueue->push(Command(VisionApi, "RefreshVision", Database));
		RefreshVisionTime = std::chrono::system_clock::now() + std::chrono::milliseconds(CheckVisionInterval);
	}
}

void Intelligence::CheckAfstandbediening()
{
	if (std::chrono::system_clock::now() > refreshAfstandBedieningTime) {
		std::vector<std::string> args;
		args.push_back(std::to_string(Database->speed));
		CommandQueue->push(Command(Sensor, "refresh", Database, args));
		CheckInfo();
		refreshAfstandBedieningTime = std::chrono::system_clock::now() + std::chrono::milliseconds(RefreshInterval);
	}
}
void Intelligence::CheckInfo()
{
	if (std::chrono::system_clock::now() > infoTime) {
		std::vector<std::string> args;
		CommandQueue->push(Command(Sensor, "info", Database, args));
		infoTime = std::chrono::system_clock::now() + std::chrono::milliseconds(infoInterval);
	}
}
void Intelligence::ExecuteLed() {
	if (std::chrono::system_clock::now() > LedTime) {
		CommandQueue->push(Command(Sensor, "TurnEyeCyan"));


		LedTime = std::chrono::system_clock::now() + std::chrono::milliseconds(LedInterval);
	}
}
void Intelligence::ExecuteDanceSi()
{
	
}
bool onoff = false;
void Intelligence::ExecuteArm()
{
//
//	std::vector<std::string> args;
//	if (std::chrono::system_clock::now() > MoveArmTime) {
//
//		if (onoff == false) {
//			CommandQueue->push(Command(Worker, "KineArmForward", Database, args));//forward is hardcoded
//
//			onoff = true;
//		}
//		else {
//			CommandQueue->push(Command(Worker, "KineArmBackward", Database, args));//forward is hardcoded
//
//			onoff = false;
//		}
//		MoveArmTime = std::chrono::system_clock::now() + std::chrono::milliseconds(ArmInterval);
//
//	}
//	//debug
	


	if (std::chrono::system_clock::now() > MoveArmTime && Database->modus == modus::arm) {
		std::vector<std::string> args;
		args.push_back(std::to_string(joy1->first));
		args.push_back(std::to_string(joy1->second));

		if (Database->joy2.first < 20) {
			CommandQueue->push(Command(Worker, "ArmLeft", Database, args));
		}
		else if (Database->joy2.first > 40) {
			CommandQueue->push(Command(Worker, "ArmRight", Database, args));
		}
		else if (Database->joy2.second > 40) {
			CommandQueue->push(Command(Worker, "KineArmForward", Database, args));//forward is hardcoded
		}
		else if (Database->joy2.second < 20) {
			CommandQueue->push(Command(Worker, "KineArmBackward", Database, args));//backward is hardcoded
		}
		else if (Database->joy1.second > 40) {
			//CommandQueue->push(Command(Worker, "KineArmUp", Database, args));
		}
		else if (Database->joy1.second < 20) {
			//CommandQueue->push(Command(Worker, "KineArmDown", Database, args));
		}


		if (Database->updateGrab == true) {
			if (Database->grab == true)
				CommandQueue->push(Command(Worker, "GrabOn", Database, args));

			if (Database->grab == false)
				CommandQueue->push(Command(Worker, "GrabOff", Database, args));

			Database->updateGrab = false;

		}
		MoveArmTime = std::chrono::system_clock::now() + std::chrono::milliseconds(ArmInterval);
	}
}
void Intelligence::ExecuteVision()
{
	if (std::chrono::system_clock::now() > ExecuteVisionTime) {


		switch (Intelligence::Database->modus)
		{
		case modus::Modus::BlueBeam:
			ExecuteBlueBeam();
			break;
		case modus::Modus::grindpad:
			break;
		case modus::Modus::poortje:
			break;
		case modus::Modus::eggtelligence:
			ExecuteEgg();
			break;
		case modus::Modus::chickenSurvivalRun:
			//function call here for chickinsurvivalrun
			break;

		case modus::Modus::DanceLi:
			break;

		case modus::Modus::DanceSi: // not really a vision function, but is the easiest way to add it
			ExecuteDanceSi();
			break;
		default:
			break;
		}
		ExecuteVisionTime = std::chrono::system_clock::now() + std::chrono::milliseconds(ExecuteVisionInterval);

	}
}
//splitting distance from coordinate
void Intelligence::SplitOn(std::string const &input, char sep, std::vector<std::string>& output) {
	std::istringstream buffer(input);
	std::string temp;
	while (std::getline(buffer, temp, sep))
		output.push_back(temp);
}



