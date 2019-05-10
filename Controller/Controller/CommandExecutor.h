#pragma once


#include "GuardedQueue.h"
#include "Command.h"
#include <string>
class CommandExecutor
{
public:
	CommandExecutor(bool &r, GuardedQueue<Command>* gq);
	~CommandExecutor();
	void Execute();

private:
	GuardedQueue<Command>* CommandQueue;
	bool* running;


};

