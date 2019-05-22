#include <iostream>
#include <string>
#include <queue>
#include "Command.h"
#include "MicroController.h" 
#include "GuardedQueue.h"
#include <chrono>
#include <thread>
#include "CommandExecutor.h"
#include "DataCollector.h"
#include "Intelligence.h"
#include "Vision.h"
bool* running = new bool(true);
GuardedQueue<Command>* Commandqueue = new GuardedQueue<Command>();
DataCollector* Datacollector = new DataCollector();

MicroController* Worker = new MicroController("/dev/ttyACM1");
MicroController* Sensor = new MicroController("/dev/ttyACM0");

int main()
{	
	std::cout << "Controller Online!" << std::endl;
	//CommandExecutor* CExe = new CommandExecutor(running, Commandqueue);
	//std::thread ExecutorThread = std::thread(&CommandExecutor::Execute, CExe);
	Intelligence* AI = new Intelligence(Datacollector, Commandqueue, running, Worker, Sensor);
	std::thread AIThread = std::thread(&Intelligence::Think, AI);

	Vision* VisionApi = new Vision();
	while (true)
	{
		std::cout << VisionApi->executeCommand("1:1:0:0") << std::endl; 
	}

	//ExecutorThread.join();
	AIThread.join();


    return 0;
}

