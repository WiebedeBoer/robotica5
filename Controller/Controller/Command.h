#pragma once
#include"Arduino.h"
#include<string>
#include<vector>
#include<iostream>
class Command
{
private:
std::string type;
std::vector<std::string> args;
public:
	Command(Arduino a, std::string t);
	Command(Arduino a, std::string t, std::vector<std::string> args);
	Arduino slave;

	~Command();
	virtual void Execute();
	

};

