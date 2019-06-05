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
	

	if (Command::type == "refresh") {
		Command::slave->SerialSend("refresh?," + args[0]);
		Command::Database->SetAfstandBedieningData(Command::slave->GetLastResponce());
		return;
	}
	if (Command::type == "GetJoystick") {
		std::cout << "1:" << std::to_string(Command::Database->GetJoy1().first) << "," << std::to_string(Command::Database->GetJoy1().second) << std::endl;
		std::cout << "2:" << std::to_string(Command::Database->GetJoy2().first) << "," << std::to_string(Command::Database->GetJoy2().second) << std::endl;
		return;
	}
	if (Command::type == "PrintLastResponce") {
		std::cout << Command::slave->GetLastResponce() << std::endl;
		return;
	}
	if (Command::type == "ArmForward") {
		Command::slave->SerialSend("servoDS?,1;1;200&5;0");
		std::cout << "The Arm is moving forward!!!:" <<args[0] <<"," <<  args[1] << std::endl;
		return;

	}
	if (Command::type == "ArmBackward") {
		Command::slave->SerialSend("servoDS?,1;2;200&5;0");
		std::cout << "The Arm is moving Backward!!!:" << args[0] << "," << args[1] << std::endl;
		return;

	}
	if (Command::type == "DriveStop") {
		Command::slave->SerialSend("motor?,0;0&0;0");
	}
	if (Command::type == "DriveForward") {
		Command::slave->SerialSend("motor?,1;"+args[0]+"&2;" + args[0] + "");
	}
	if (Command::type == "DriveBackward") {
		Command::slave->SerialSend("motor?,2;" + args[0] + "&2;" + args[0] + "");
	}
	if (Command::type == "DriveLeft") {
		Command::slave->SerialSend("motor?,2;" + args[0] + "&1;" + args[0] + "");
	}if (Command::type == "DriveRight") {
		Command::slave->SerialSend("motor?,1;" + args[0] + "&2;" + args[0] + "");
	}
	if (Command::type == "chickenSurvivalRun") {
		std::string temp = Command::VisionSlave->executeCommand("2:0:0");
		
		Command::Database->wedstrijd.chickenSurvivalRun = Command::VisionSlave->executeCommand("2:0:0");
		std::cout <<"chickenSurvivalRun: " <<Command::Database->wedstrijd.chickenSurvivalRun << std::endl;
		return;
	}
	if (Command::type == "eggDistance") {
		std::string temp = Command::VisionSlave->executeCommand("3:0:0");
		
		Command::Database->wedstrijd.eggDistance = Command::VisionSlave->executeCommand("3:0:0");
		std::cout << "EggDistance: " << Command::Database->wedstrijd.eggDistance << std::endl;
		return;
	}
	if (Command::type == "qrDistance") {
		std::string temp = Command::VisionSlave->executeCommand("3:1:0");
		
		Command::Database->wedstrijd.qrDistance = Command::VisionSlave->executeCommand("3:1:0");
		std::cout << "qrDistance: " << Command::Database->wedstrijd.qrDistance << std::endl;
		return;
	}
	if (Command::type == "GripperVision") {
		//Command::Database->kwalificatie.eiGripper = Command::VisionSlave->executeCommand("0:3:0:0");
		//std::cout << "eiGripper: " << Command::Database->kwalificatie.eiGripper << std::endl;
		//Command::Database->wedstrijd.eggDistance = Command::VisionSlave->executeCommand("0:3:0:0");
		Command::Database->wedstrijd.eggDistance = Command::VisionSlave->executeCommand("3:0:0");
		std::cout << "GripperVision: " << Command::Database->wedstrijd.eggDistance << std::endl;
		return;
	}
}