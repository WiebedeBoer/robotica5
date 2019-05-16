#pragma once
#include "GuardedQueue.h"
#include "Command.h"
#include <string>
#include <thread>
#include <chrono>
class CommandExecutor
{
public:
	CommandExecutor(bool *r, GuardedQueue<Command>* gq);
	~CommandExecutor();
	void Execute();

private:
	bool QueueIsEmpty();
	GuardedQueue<Command>* CommandQueue;
	bool* running;



};

