#include "Command.h"
#include "Angles.h"
#include <string>
#include <stdio.h>

Command::Command()
{
}

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
		if (Database->shutdown == "1") {
			std::string cmd = "sudo shutdown now";
			char* ccmd = convertstrtochar(cmd);
			system(ccmd);
			delete(ccmd);
		}
		return;
	}
	if (Command::type == "sleep") {
		if (args.size() == 1) {
			std::this_thread::sleep_for(std::chrono::milliseconds(std::stoi(args[0])));
		}
		else
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));

		}
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
		return;
	}
	if (Command::type == "ArmRight") {
		Command::slave->SerialSend("servoDS?,1;2;100&5;0");
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
	}if (Command::type == "GrabOn") {
		Command::slave->SerialSend("servoS?,5;250;50&;60;100");
		return;
	}if (Command::type == "GrabOff") {
		Command::slave->SerialSend("servoS?,5;0;50&;60;100");
		return;
	}
	if (Command::type == "KineArmForward") {
		Command::slave->SerialSend("servoS?,4;300;32&6;0;30");

		Command::slave->SerialSend("servoS?,2;300;32&3;100;32");//servocommand,ID;POS;SPEED; //servoS?,1;100;50&5;0;100|10 //ID;POS;SPEED
		//second command to keep height steady
		return;
	}
	if (Command::type == "KineArmBackward") {
		Command::slave->SerialSend("servoS?,4;530;42&6;0;50");

		Command::slave->SerialSend("servoS?,2;170;32&3;250;32");//servocommand,ID;POS;SPEED; //servoS?,1;100;50&5;0;100|10 //ID;POS;SPEED
		//second command to keep height steady
		return;
	}

	if (Command::type == "KineArmUp") {
		ArmMove armsmoving;
		Angles dlg;

		char buffer[100];
		//offsetting
			//double joyOffset = (stoi(args[0]) - 40) + armsmoving.yoffset; //offset from joystick
		double joyOffset = armsmoving.yoffset;
		double Targety = armsmoving.Ypos;
		Targety = armsmoving.Ypos + joyOffset;
		double Targetx = armsmoving.Xpos;
		if (Targetx > 40 && Targety > 200) {
			//first command to move arm
			std::vector<int> noutput = dlg.Gonio(Targetx, Targety);
			if (noutput.size() == 3) {
				armsmoving.Xpos = Targetx;
				armsmoving.Ypos = Targety;
				std::cout << " 1" << noutput[0] << " 2" << noutput[1] << " 3" << noutput[2] << std::endl;

				sprintf(buffer, "servoS?,2;%d;16&3;%d;32", noutput[0], noutput[1]);
				Command::slave->SerialSend(buffer);//servocommand,ID;POS;SPEED; //servoS?,1;100;50&5;0;100|10 //ID;POS;SPEED
				//second command to keep height steady
				sprintf(buffer, "servoS?,6;0;16&4;0;%d", noutput[2]);
				Command::slave->SerialSend(buffer);
				std::cout << "The Arm is moving forward!!!:" << args[0] << "," << args[1] << std::endl;

			}
		}
		return;
	}

	if (Command::type == "KineArmDown") {
		ArmMove armsmoving;
		Angles dlg;
		char buffer[100];
		//offsetting
		//double joyOffset = (20 - stoi(args[0])) + armsmoving.yoffset; //offset from joystick
		double joyOffset = armsmoving.yoffset;
		double Targety = armsmoving.Ypos;
		Targety = armsmoving.Ypos - joyOffset;
		double Targetx = armsmoving.Xpos;
		if (Targetx > 40 && Targety > 210) {
			//first command to move arm
			std::vector<int> noutput = dlg.Gonio(Targetx, Targety);
			if (noutput.size() == 3) {
				armsmoving.Xpos = Targetx;
				armsmoving.Ypos = Targety;
				std::cout << " 1" << noutput[0] << " 2" << noutput[1] << " 3" << noutput[2] << std::endl;

				sprintf(buffer, "servoS?,2;%d;16&3;%d;32", noutput[0], noutput[1]);
				Command::slave->SerialSend(buffer);
				//second command to keep height steady
				sprintf(buffer, "servoS?,6;0;16&4;0;%d", noutput[2]);
				Command::slave->SerialSend(buffer);
				std::cout << "The Arm is moving Backward!!!:" << args[0] << "," << args[1] << std::endl;

			}
		}
		return;
	}

	if (Command::type == "KineArmLeft") {
		int OffsetLeft = 8; //hardcoded target, must be from python vision for autonomous
		char buffer[100];
		sprintf(buffer, "servoS?,1;%d;32&6;0;0", OffsetLeft);
		Command::slave->SerialSend(buffer); //ID;POS;SPEED
		std::cout << " 1" << OffsetLeft << std::endl;

		std::cout << "The Arm is moving kineLeft!!!:" << args[0] << "," << args[1] << std::endl;
		return;
	}
	if (Command::type == "KineArmRight") {
		int OffsetRight = 8;  //hardcoded target, must be from python vision for autonomous
		char buffer[100];
		sprintf(buffer, "servoS?,1;%d;32&6;0;0", OffsetRight);
		std::cout << " 1" << OffsetRight << std::endl;

		Command::slave->SerialSend(buffer); //ID;POS;SPEED
		std::cout << "The Arm is moving kineRight!!!:" << args[0] << "," << args[1] << std::endl;
		return;
	}
	if (Command::type == "Gripper") {
		int OffsetGrip = 186;  //hardcoded target, must be from python vision for autonomous
		char buffer[100];
		sprintf(buffer, "servoS?,5;%d;16&6;0;0", OffsetGrip);
		Command::slave->SerialSend(buffer); //ID;POS;SPEED
		std::cout << "The Arm is moving Right!!!:" << args[0] << "," << args[1] << std::endl;
		return;
	}
	if (Command::type == "GripperLoose") {
		int OffsetGrip = 250;  //hardcoded target, must be from python vision for autonomous
		char buffer[100];
		sprintf(buffer, "servoS?,5;%d;16&6;0;0", OffsetGrip);
		Command::slave->SerialSend(buffer); //ID;POS;SPEED
		return;
	}
	if (Command::type == "DriveStop") {
		Command::slave->SerialSend("motor?,0;0&0;0");
		return;
	}
	if (Command::type == "DriveForward") {
		Command::slave->SerialSend("motor?,1;" + args[0] + "&1;" + args[0] + "");
		return;
	}
	if (Command::type == "DriveBackward") {
		Command::slave->SerialSend("motor?,2;" + args[0] + "&2;" + args[0] + "");
		return;
	}
	if (Command::type == "DriveLeft") {
		Command::slave->SerialSend("motor?,1;" + args[0] + "&2;0");
		return;
	}if (Command::type == "DriveRight") {
		Command::slave->SerialSend("motor?,2;0&1;" + args[0] + "");
		return;
	}if (Command::type == "speak_normal") {
		Command::tts.speak_normal(args[0]);
		return;
	}if (Command::type == "speak_happy") {
		Command::tts.speak_happy(args[0]);
		return;
	}if (Command::type == "speak_angry") {
		Command::tts.speak_angry(args[0]);
		return;
	}if (Command::type == "speak_pitch") {
		Command::tts.give_pitch();
		return;
	}if (Command::type == "speak_random") {
		Command::tts.random_quote();
		return;
	}if (Command::type == "TurnEyeCyan") {
		Command::slave->SerialSend("eyeCyan?,0");
		return;
	}if (Command::type == "TurnEyeRed") {
		Command::slave->SerialSend("eyeRed?,0");
		return;
	}if (Command::type == "TurnEyeBlue") {
		Command::slave->SerialSend("eyeBlue?,0");
		return;
	}if (Command::type == "TurnEyeWhite") {
		Command::slave->SerialSend("eyeWhite?,0");
		return;
	}if (Command::type == "TurnEyePink") {
		Command::slave->SerialSend("eyePink?,0");
		return;
	}

	if (Command::type == "Head1") {
		Command::slave->SerialSend("headPos1?,0");
		return;
	}
	if (Command::type == "Head2") {
		Command::slave->SerialSend("headPos2?,0");
		return;
	}
	if (Command::type == "Head3") {
		Command::slave->SerialSend("headPos3?,0");
		return;
	}
	//if (Command::type == "Head4") {
	//	Command::slave->SerialSend("headPos3?,0");
	//	return;
	//}
	if (Command::type == "Eyes1") {
		Command::slave->SerialSend("headPos4?,0");
		return;
	}
	if (Command::type == "Eyes2") {
		Command::slave->SerialSend("headPos3?,0");
		return;
	}
	if (Command::type == "Eyes3") {
		Command::slave->SerialSend("headPos3?,0");
		return;
	}

	if (Command::type == "RefreshVision") {

		switch (Command::Database->modus)
		{
		case modus::Modus::BlueBeam:
			Command::Database->kwalificatie.bluebeam = Command::VisionSlave->executeCommand("1:0:0");
			std::cout << "BlueBeam: " << Command::Database->kwalificatie.bluebeam << std::endl;
			break;
		case modus::Modus::grindpad:
			break;
		case modus::Modus::poortje:
			break;
		case modus::Modus::eggtelligence:
			Command::Database->kwalificatie.eggLocation = Command::VisionSlave->executeCommand("3:0:0");
			std::cout << "EggDistance: " << Command::Database->kwalificatie.eggLocation << std::endl;
			Command::Database->wedstrijd.qrDistance = Command::VisionSlave->executeCommand("3:1:0");
			std::cout << "qrdistance: " << Command::Database->wedstrijd.qrDistance << std::endl;
			Command::Database->wedstrijd.findChickin = Command::VisionSlave->executeCommand("3:2:0");
			std::cout << "FindChicken: " << Command::Database->wedstrijd.findChickin << std::endl;
			break;
		case modus::Modus::chickenSurvivalRun:
			Command::Database->wedstrijd.chickenSurvivalRun = Command::VisionSlave->executeCommand("2:0:0");
			std::cout << "chickenSurvivalRun: " << Command::Database->wedstrijd.chickenSurvivalRun << std::endl;
			break;
		case modus::Modus::DanceSi:
			Command::Database->wedstrijd.tape = Command::VisionSlave->executeCommand("0:0:0");
			std::cout << "Tape: " << Command::Database->wedstrijd.tape << std::endl;

			break;
		default:
			break;
		}
		return;
	}

	if (Command::type == "info") {
		Command::slave->SerialSend("info?,");
		Command::Database->SetSensorInfo(Command::slave->GetLastResponce());
		return;
	}
}

char* Command::convertstrtochar(std::string s) {
	int i;
	const int x = s.length();
	char* p = new char[x];
	for (i = 0; i < s.length(); i++) {
		p[i] = s[i];
	}
	return p;
}
