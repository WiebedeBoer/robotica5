#pragma once
#include"Arduino.h"
#include<string>
#include<vector>
#include<iostream>
#include"DataCollector.h"
class Command
{
private:
std::string type;
std::vector<std::string> args;
DataCollector* Datacollector;
public:
	Command(Arduino* a, std::string t);
	Command(Arduino* a, std::string t,DataCollector* DC);
	Command(Arduino* a, std::string t,DataCollector* DC, std::vector<std::string> args);
	Arduino* slave;

	~Command();
	virtual void Execute();
	

};

