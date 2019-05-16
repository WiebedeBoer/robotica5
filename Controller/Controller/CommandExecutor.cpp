#include "CommandExecutor.h"



void CommandExecutor::Execute()
{	
	while (*running == true) {
		if (QueueIsEmpty()) {
			std::this_thread::sleep_for(std::chrono::milliseconds(20));
		}
		CommandQueue->GetFront().Execute();
	}
}

bool CommandExecutor::QueueIsEmpty()
{
	if (CommandQueue->GetSize() == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

CommandExecutor::CommandExecutor(bool *r, GuardedQueue<Command>* gq)
{
	running = r;
	CommandQueue = gq;
}


CommandExecutor::~CommandExecutor()
{
}
