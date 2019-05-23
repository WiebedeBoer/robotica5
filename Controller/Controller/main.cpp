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
GuardedQueue<Command>* VisionQueue = new GuardedQueue<Command>();
DataCollector* Datacollector = new DataCollector();

MicroController* Worker = new MicroController("/dev/ttyACM1");
MicroController* Sensor = new MicroController("/dev/ttyACM0");

Vision* VisionApi = new Vision();

int main()
{	
	std::cout << "Controller Online!" << std::endl;
	CommandExecutor* CExe = new CommandExecutor(running, Commandqueue);
	std::thread ExecutorThread = std::thread(&CommandExecutor::Execute, CExe);
	Intelligence* AI = new Intelligence(Datacollector, Commandqueue, VisionQueue, running, Worker, Sensor, VisionApi);
	std::thread AIThread = std::thread(&Intelligence::Think, AI);


	CommandExecutor* VisionExecutor = new CommandExecutor(running, VisionQueue);

	ExecutorThread.join();
	AIThread.join();

    return 0;
}
