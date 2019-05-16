#include <iostream>
#include <string>
#include <queue>
#include "Command.h"
#include "Arduino.h" 
#include "GuardedQueue.h"
#include <chrono>
#include <thread>
#include "CommandExecutor.h"
#include "DataCollector.h"
#include "Intelligence.h"
bool* running = new bool(true);
GuardedQueue<Command>* Commandqueue = new GuardedQueue<Command>();
DataCollector* Datacollector = new DataCollector();

Arduino* Worker = new Arduino("/dev/ttyACM1");
Arduino* Sensor = new Arduino("/dev/ttyACM0");

int main()
{	
	std::cout << "Controller Online!" << std::endl;
	CommandExecutor* CExe = new CommandExecutor(running, Commandqueue);
	std::thread ExecutorThread = std::thread(&CommandExecutor::Execute, CExe);
	Intelligence* AI = new Intelligence(Datacollector, Commandqueue, running, Worker, Sensor);
	std::thread AIThread = std::thread(&Intelligence::Think, AI);

	ExecutorThread.join();
	AIThread.join();


    return 0;
}

