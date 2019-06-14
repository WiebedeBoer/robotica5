#include "Command.h"
#include "Angles.h"

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

Command::Command(MicroController* a, std::string t, DataCollector* DC)
{
	Command::slave = a;
	Command::type = t;
	Command::Database = DC;
}

Command::Command(MicroController* a, std::string t, DataCollector* DC, std::vector<std::string> ar)
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
	if (Command::type == "ArmLeft") {
		Command::slave->SerialSend("servoDS?,1;1;100&5;0");
		std::cout << "The Arm is moving Left!!!:" << args[0] << "," << args[1] << std::endl;
		return;
	}
	if (Command::type == "ArmRight") {
		Command::slave->SerialSend("servoDS?,1;2;100&5;0");
		std::cout << "The Arm is moving Right!!!:" << args[0] << "," << args[1] << std::endl;
		return;
	}
	if (Command::type == "ArmForward") {
		Command::slave->SerialSend("servoDS?,2;1;100&5;0");
		std::cout << "The Arm is moving forward!!!:" << args[0] << "," << args[1] << std::endl;
		return;
	}
	if (Command::type == "ArmBackward") {
		Command::slave->SerialSend("servoDS?,2;2;100&5;0");
		std::cout << "The Arm is moving Backward!!!:" << args[0] << "," << args[1] << std::endl;
		return;
	}

	if (Command::type == "KineArmForward") {
		double Targetx = 5.0;
		double Targety = 5.0;
		Angles dlg;
		dlg.Gonio(Targetx, Targety);
		Command::slave->SerialSend("servoS?,1;100;50&5;0;100");//servocommand;ID;POS;SPEED; //servoS?,1;100;50&5;0;100|10 //ID;POS;SPEED
		std::cout << "The Arm is moving forward!!!:" << args[0] << "," << args[1] << std::endl;
		return;
	}
	if (Command::type == "DriveStop") {
		Command::slave->SerialSend("motor?,0;0&0;0");
		return;
	}
	if (Command::type == "DriveForward") {
		std::string cmd = "motor?,1;" + args[0] + "&2;" + args[0] + "";
		Command::slave->SerialSend("motor?,1;" + args[0] + "&2;" + args[0] + "");
		return;
	}
	if (Command::type == "DriveBackward") {
		Command::slave->SerialSend("motor?,2;" + args[0] + "&2;" + args[0] + "");
		return;
	}
	if (Command::type == "DriveLeft") {
		Command::slave->SerialSend("motor?,2;" + args[0] + "&1;" + args[0] + "");
		return;
	}if (Command::type == "DriveRight") {
		Command::slave->SerialSend("motor?,1;" + args[0] + "&2;" + args[0] + "");
		return;
	}


	if (Command::type == "RefreshVision") {
		switch (Command::Database->modus)
		{
		case 0:
			Command::Database->kwalificatie.vision = Command::VisionSlave->executeCommand("1:0:0");
			std::cout << "BlueBeam: " << Command::Database->kwalificatie.vision << std::endl;
			break;
		case 1:
			break;
		case 2:
			break;
		case 3:
			Command::Database->wedstrijd.eggDistance = Command::VisionSlave->executeCommand("3:0:0");
			std::cout << "EggDistance: " << Command::Database->wedstrijd.eggDistance << std::endl;
			break;
		case 4:
			Command::Database->wedstrijd.chickenSurvivalRun = Command::VisionSlave->executeCommand("2:0:0");
			std::cout << "chickenSurvivalRun: " << Command::Database->wedstrijd.chickenSurvivalRun << std::endl;
			break;
		case 5:
			Command::Database->wedstrijd.qrDistance = Command::VisionSlave->executeCommand("3:1:0");
			std::cout << "qrdistance: " << Command::Database->wedstrijd.qrDistance << std::endl;
			break;
		default:
			break;
		}
		return;
	}
}