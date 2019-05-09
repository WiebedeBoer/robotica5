#pragma once
#include "Command.h"
#include <string>
class Move :
	public Command
{
private:
	Arduino slave;
public:
	Move(Arduino a, std::string 
		t);
	~Move();
	void Execute() override;

};

