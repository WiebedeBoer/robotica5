#include "Command.h"



Command::Command(Vision * v, std::string t)
{
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
	Command::Datacollector = DC;
}

Command::Command(MicroController* a, std::string t,DataCollector* DC, std::vector<std::string> ar)
{
	Command::slave = a;
	Command::type = t;
	Command::Datacollector = DC;
	Command::args = ar;
}

Command::~Command()
{

}

void Command::Execute() {

	if (Command::type == "refresh?") {
		Command::slave->SerialSend("refresh?," + args[0]);
		Command::Datacollector->SetAfstandBedieningData(Command::slave->GetLastResponce());
	}
	if (Command::type == "GetJoystick") {
		std::cout << "1:" << std::to_string(Command::Datacollector->GetJoy1().first) << "," << std::to_string(Command::Datacollector->GetJoy1().second) << std::endl;
		std::cout << "2:" << std::to_string(Command::Datacollector->GetJoy2().first) << "," << std::to_string(Command::Datacollector->GetJoy2().second) << std::endl;
	}
	if (Command::type == "PrintLastResponce") {
		std::cout << Command::slave->GetLastResponce() << std::endl;
	}
	if (Command::type == "MoveArm") {
		std::cout << "The Arm is moving!!!:" <<args[0] <<"," <<  args[1] << ":" << args[2] << "," << args[3] << std::endl;
	}
	if (Command::type == "Vision") {
		Command::VisionSlave->executeCommand("HalloWorld");
	}
}