#include "CommandExecutor.h"


void CommandExecutor::Execute()
{
	
	while (*running == true) {
		CommandQueue->GetFront().Execute();
	}
}

CommandExecutor::CommandExecutor(bool &r, GuardedQueue<Command> &gq)
{
	running = &r;
	CommandQueue = &gq;
}


CommandExecutor::~CommandExecutor()
{
}
