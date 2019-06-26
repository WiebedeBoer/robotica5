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
int RefreshInterval = 500;
int PrintInterval = 510;
int ArmInterval = 500;
int DriveInterval = 200;
int CheckVisionInterval = 1000000;
int ExecuteVisionInterval = 1500000;
int GripperInterval = 500;
int SpeakInterval = 30000;
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

//chicken trap?
void Intelligence::ExecuteChickinSurivalRun()
{
	//autonomous stairs up
	std::vector<std::string> args;
	args.push_back("");
	args[0] = "64";
	CommandQueue->push(Command(Worker, "DriveBackward", Database, args));
	//else on joystick
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

//eggtelligence match
void Intelligence::ExecuteEgg() {
	bool driverEgg = false;
	bool armEgg = false;
	bool trayEgg = false;
	bool dropEgg = false;

	//step 0, set qr for tray, do this once

	//repeat steps max 3 times
	for (int inc = 0; inc < 3; inc++) {
		//step 1, find egg
		driverEgg = DriveEggtelligence();
		//stop at tape (mode tape: false, both, left, right), Jesse
		//go around at chicken, Robin
		if (driverEgg == true) {
			armGrip = "fasten";
			armEgg = ArmEggtelligence(armGrip);
		}			
		//step 2.1, arm forward
		//step 2.2, lower arm
		//step 2.3, close gripper and pick up egg
		//step 2.4, raise arm
		//stop at tape (mode tape: false, both, left, right), Jesse
		TapeHelper();
		//go around at chicken, Robin
		ChickenHelper();


		if (armEgg == true) {
			trayEgg = TrayEggtelligence();
		}			
		//step 3, drive towards tray
		if (trayEgg == true) {
			armGrip = "loose";
			dropEgg = ArmEggtelligence(armGrip);
		}			
		//step 4.1, see qr is near tray, lower arm
		//step 4.2 open gripper, drop egg in tray
		if (dropEgg == true) {
			driverEgg = false;
			armEgg = false;
			trayEgg = false;
			dropEgg = false;
		}
	}

}

//eggtelligence match step 0, fill in qr for city name tray, to begin eggtelligence

//eggtelligence match step 1, find egg
bool Intelligence::DriveEggtelligence() {
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
	return true;
}

//eggtelligence match, move arm to, step 2 pick up egg, step 4 drop off egg
bool Intelligence::ArmEggtelligence(std::string armGrip) {
	std::vector<std::string> args;
	args.push_back("");
	//step 2.1, arm forward
	CommandQueue->push(Command(Worker, "KineArmForward", Database, args));
	CommandQueue->push(Command(Worker, "Sleep", Database, args)); //pause 1 sec
	//step 2.2, lower arm
	CommandQueue->push(Command(Worker, "KineArmDown", Database, args));
	if (armGrip =="loose") {
		//step 2.3, close gripper and pick up egg
		CommandQueue->push(Command(Worker, "Gripper", Database, args));
	}
	else {
		//step 4.3 open gripper, drop egg in tray
		CommandQueue->push(Command(Worker, "GripperLoose", Database, args));
	}
	CommandQueue->push(Command(Worker, "Sleep", Database, args)); //pause 1 sec
	//step 2.4, raise arm
	CommandQueue->push(Command(Worker, "KineArmUp", Database, args));
	CommandQueue->push(Command(Worker, "Sleep", Database, args)); //pause 1 sec
	//step 2.4, arm backward
	CommandQueue->push(Command(Worker, "KineArmBackward", Database, args));
	return true;
}

//eggtelligence match step 3, find tray
bool Intelligence::TrayEggtelligence() {
	//step 3, drive towards tray
	std::vector<std::string> args;
	args.push_back("");
	int distance = 999; //distance
	int horizontal = 0; //horizontal coordinate of tray
	if (!Intelligence::Database->wedstrijd.qrDistance.empty()) {
		std::string s = Intelligence::Database->wedstrijd.qrDistance; //qr distance
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
				std::cout << "QRDistanceIsToSmall" << std::endl;
				CommandQueue->push(Command(Worker, "DriveStop", Database, args));
			}
		}
	}
	return true;
}

//eggtelligence tape helper
void Intelligence::TapeHelper() {
	std::vector<std::string> out;
	//std::string s = Intelligence::Database->kwalificatie.bluebeam;
	std::string s;

	SplitOn(ref(s), ':', ref(out));

	try {
		if (out[0] != "False") {
			std::cout << "tape found" << '\n';

				std::vector<std::string> args;
				args.push_back("");
				CommandQueue->push(Command(Worker, "DriveLeft", Database, args));
			
		}
	}
	catch (float e) {
		std::cout << "Stof error occurred. Exception" << e << '\n';
	}
}

//eggtelligence chicken helper
void Intelligence::ChickenHelper() {
	std::vector<std::string> out;
	//std::string s = Intelligence::Database->kwalificatie.bluebeam;
	std::string s;

	SplitOn(ref(s), ':', ref(out));

	try {
		if (out[0] != "False") {
			std::cout << "chicken found" << '\n';
			std::vector<std::string> args;
			args.push_back("");
			CommandQueue->push(Command(Worker, "DriveLeft", Database, args));
		}
	}
	catch (float e) {
		std::cout << "Stof error occurred. Exception" << e << '\n';
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

		try {
			if (out[0] != "False") {
				distance = std::stoi(out[0]);

				// Write if not initialized
				if (Database->horizontalBlueBeam == std::numeric_limits<float>::max())
					Database->horizontalBlueBeam = std::stof(out[1]);

				// Write if new value
				if (abs(Database->horizontalBlueBeam - 10) > abs(std::stof(out[1])) || abs(Database->horizontalBlueBeam + 10) < abs(std::stof(out[1])))
					Database->horizontalBlueBeam = std::stof(out[1]);
			}
		}
		catch (float e) {
			std::cout << "Stof error occurred. Exception" << e << '\n';
		}

		if (Database->horizontalBlueBeam != std::numeric_limits<float>::max()) {
			//if near and in sight, drive
			if (distance >= 5 && distance < 210) {
				auto argsChange = [](std::string speed, std::vector<std::string> args) { args[0] = speed; return args; };

				//left
				if (Database->horizontalBlueBeam < -50)
					CommandQueue->push(Command(Worker, "DriveLeft", Database, argsChange("128", args)));

				//right
				else if (Database->horizontalBlueBeam > 50)
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

	int slowSpeed = 32;
	int midSpeed = 64;
	int highSpeed = 128;
	if (std::chrono::system_clock::now() > DriveTime && Database->modus != modus::arm) {
		DriveTime = std::chrono::system_clock::now() + std::chrono::milliseconds(DriveInterval);

		//if (Database->GetJoy2().first != 0 && Database->GetJoy2().second != 0) {
		std::vector<std::string> args;
		args.push_back("");
		std::pair<int, int>* Tempjoy2 = new std::pair<int, int>(Database->GetJoy2());

		if (Database->GetJoy2().second > 35) {//driveleft
			args[0] = slowSpeed;
			if (Database->GetJoy2().second > 45) {
				args[0] = midSpeed;
				if (Database->GetJoy2().second > 55) {
					args[0] = highSpeed;
				}
			}
			CommandQueue->push(Command(Worker, "DriveForward", Database, args));

			return;

		}
		if (Database->GetJoy2().second < 25) {//DriveRight
			args[0] = slowSpeed;
			if (Database->GetJoy2().second < 20) {
				args[0] = midSpeed;
				if (Database->GetJoy2().second < 10) {
					args[0] = highSpeed;
				}
			}
			CommandQueue->push(Command(Worker, "DriveBackward", Database, args));

			return;

		}

		if (Database->GetJoy2().first > 35) {//DriveForward
			args[0] = slowSpeed;
			if (Database->GetJoy2().first > 45) {
				args[0] = midSpeed;
				if (Database->GetJoy2().first > 55) {
					args[0] = highSpeed;
				}
			}
			CommandQueue->push(Command(Worker, "DriveRight", Database, args));

			return;

		}
		if (Database->GetJoy2().first < 25) {//DriveBackward
			args[0] = slowSpeed;
			if (Database->GetJoy2().first < 20) {
				args[0] = midSpeed;
				if (Database->GetJoy2().first < 10) {
					args[0] = highSpeed;
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
		//}
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
void Intelligence::ExecuteArm()
{
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
			CommandQueue->push(Command(Worker, "KineArmForward", Database, args));
		}
		else if (Database->joy2.second < 20) {
			CommandQueue->push(Command(Worker, "KineArmBackward", Database, args));
		}
		else if (Database->joy1.second > 40) {
			CommandQueue->push(Command(Worker, "KineArmUp", Database, args));
		}
		else if (Database->joy1.second < 20) {
			CommandQueue->push(Command(Worker, "KineArmDown", Database, args));
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



