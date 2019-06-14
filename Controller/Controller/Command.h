#pragma once
#include"MicroController.h"
#include<string>
#include<vector>
#include<iostream>
#include"DataCollector.h"
#include "Vision.h"
#include "TextToSpeach.h"
class Command
{
private:
std::string type; //defines this command
std::vector<std::string> args;//arguments added to this command
DataCollector* Database;
TextToSpeach tts;//used for texttospeach
public:
	Command(Vision* v, std::string t, DataCollector* DC);
	Command(MicroController* a, std::string t);
	Command(MicroController* a, std::string t,DataCollector* DC);
	Command(MicroController* a, std::string t,DataCollector* DC, std::vector<std::string> args);
	MicroController* slave;
	Vision* VisionSlave;
	~Command();
	virtual void Execute();

};

