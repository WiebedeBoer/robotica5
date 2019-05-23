#include "Command.h"

Command::Command(Vision * v, std::string t, DataCollector* DC)
{
	Command::VisionSlave = v;
	Command::type = t;
	Command::Database = DC;
}

Command::Command(MicroController * a, std::string t)
{
	Command::slave = a;
	Command::type = t;
}

Command::Command(MicroController* a , std::string t, DataCollector* DC)
{
	Command::slave = a;
	Command::type = t;
	Command::Database = DC;
}

Command::Command(MicroController* a, std::string t,DataCollector* DC, std::vector<std::string> ar)
{
	Command::slave = a;
	Command::type = t;
	Command::Database = DC;
	Command::args = ar;
}

Command::~Command()
{

}

void Command::Execute() {

	if (Command::type == "refresh?") {
		Command::slave->SerialSend("refresh?," + args[0]);
		Command::Database->SetAfstandBedieningData(Command::slave->GetLastResponce());
	}
	if (Command::type == "GetJoystick") {
		std::cout << "1:" << std::to_string(Command::Database->GetJoy1().first) << "," << std::to_string(Command::Database->GetJoy1().second) << std::endl;
		std::cout << "2:" << std::to_string(Command::Database->GetJoy2().first) << "," << std::to_string(Command::Database->GetJoy2().second) << std::endl;
	}
	if (Command::type == "PrintLastResponce") {
		std::cout << Command::slave->GetLastResponce() << std::endl;
	}
	if (Command::type == "ArmForward") {
		Command::slave->SerialSend("servoDS?,1;1;200&5;0");

		std::cout << "The Arm is moving forward!!!:" <<args[0] <<"," <<  args[1] << ":" << args[2] << "," << args[3] << std::endl;
	}
	if (Command::type == "ArmBackward") {
		Command::slave->SerialSend("servoDS?,1;2;200&5;0");

		std::cout << "The Arm is moving Backward!!!:" << args[0] << "," << args[1] << ":" << args[2] << "," << args[3] << std::endl;
	}
	if (Command::type == "chickenSurvivalRun") {
		Command::Database->wedstrijd.chickenSurvivalRun = Command::VisionSlave->executeCommand("1:0:0:0");
		std::cout <<"chickenSurvivalRun: " <<Command::Database->wedstrijd.chickenSurvivalRun << std::endl;
	}
	if (Command::type == "eggDistance") {
		Command::Database->wedstrijd.eggDistance = Command::VisionSlave->executeCommand("1:1:0:0");
		std::cout << "EggDistance: " << Command::Database->wedstrijd.eggDistance << std::endl;
	}
	if (Command::type == "qrDistance") {
		Command::Database->wedstrijd.qrDistance = Command::VisionSlave->executeCommand("1:1:1:0");
		std::cout << "qrDistance: " << Command::Database->wedstrijd.qrDistance << std::endl;
	}
	if (Command::type == "pitch") {
		Command::Database->kwalificatie.pitch = Command::VisionSlave->executeCommand("1:1:0:0");
		std::cout << "pitch: " << Command::Database->kwalificatie.pitch << std::endl;
	}
	if (Command::type == "poort") {
		Command::Database->kwalificatie.poort = Command::VisionSlave->executeCommand("0:1:0:0");
		std::cout << "poort: " << Command::Database->kwalificatie.poort << std::endl;
	}
	if (Command::type == "grindpad") {
		Command::Database->kwalificatie.grindpad = Command::VisionSlave->executeCommand("0:2:0:0");
		std::cout << "grindpad: " << Command::Database->kwalificatie.grindpad << std::endl;
	}
	if (Command::type == "eiGripper") {
		Command::Database->kwalificatie.eiGripper = Command::VisionSlave->executeCommand("0:3:0:0");
		std::cout << "eiGripper: " << Command::Database->kwalificatie.eiGripper << std::endl;
	}
	if (Command::type == "vision") {
		Command::Database->kwalificatie.vision = Command::VisionSlave->executeCommand("0:4:0:0");
		std::cout << "vision: " << Command::Database->kwalificatie.vision << std::endl;
	}
}