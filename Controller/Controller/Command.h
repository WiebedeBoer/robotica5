#pragma once
#include"MicroController.h"
#include<string>
#include<vector>
#include<iostream>
#include"DataCollector.h"
#include "Vision.h"
class Command
{
private:
std::string type;
std::vector<std::string> args;
DataCollector* Datacollector;
public:
	Command(Vision* v, std::string t);
	Command(MicroController* a, std::string t);
	Command(MicroController* a, std::string t,DataCollector* DC);
	Command(MicroController* a, std::string t,DataCollector* DC, std::vector<std::string> args);
	MicroController* slave;
	Vision* VisionSlave;
	~Command();
	virtual void Execute();
	

};

