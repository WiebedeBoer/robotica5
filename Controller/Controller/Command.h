#pragma once
#include"MicroController.h"
#include<string>
#include<vector>
#include<iostream>
#include"DataCollector.h"
#include "Vision.h"
#include "TextToSpeach.h"
#include "Angles.h"

//arm datacollector
struct ArmMove {
	double xoffset = 6.0; //offset x
	double yoffset = 6.0; //offset y
	double Xpos = 225.0; //hardcoded target, must be from python vision for autonomous
	double Ypos = 235.0; //hardcoded target, must be from python vision for autonomous	
};

class Command
{
private:
std::string type; //defines this command
std::vector<std::string> args;//arguments added to this command
DataCollector* Database;
TextToSpeach tts;//used for texttospeach
public:
	Command();
	Command(Vision* v, std::string t, DataCollector* DC);
	Command(MicroController* a, std::string t);
	Command(MicroController* a, std::string t,DataCollector* DC);
	Command(MicroController* a, std::string t,DataCollector* DC, std::vector<std::string> args);
	MicroController* slave;
	Vision* VisionSlave;
	~Command();
	virtual void Execute();
	char* convertstrtochar(std::string s);
};

