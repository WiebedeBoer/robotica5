#include "Command.h"



Command::Command(Arduino a , std::string t)
{
	Command::slave = a;
	Command::type = t;
}

Command::Command(Arduino a, std::string t, std::vector<std::string> ar)
{
	Command::slave = a;
	Command::type = t;
	Command::args = ar;
}

Command::~Command()
{

}

void Command::Execute() {
	//std::cout << Command::slave.UsbPort << std::endl;

	if (Command::type == "LedOn") {
		Command::slave.SerialSend("LedOn");
	}
	if (Command::type == "LedOff") {
		Command::slave.SerialSend("LedOff");
	}
	if (Command::type == "Led?") {
		Command::slave.SerialSend("Led?");
	}
}