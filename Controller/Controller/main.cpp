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

bool* running = new bool(true);//when set to false, program stops
GuardedQueue<Command>* Commandqueue = new GuardedQueue<Command>();//queue for short commands, moslty executed on microcontrollers
GuardedQueue<Command>* VisionQueue = new GuardedQueue<Command>();//queue for vision commands, vision is slow, so is this queue
DataCollector* Datacollector = new DataCollector();//object to store all shared data

MicroController* Worker = new MicroController("/dev/ttyACM1");//worker slave
MicroController* Sensor = new MicroController("/dev/ttyACM0");//sensor slave

Vision* VisionApi = new Vision();//vision socket connection

int main()
{	
	std::cout << "Controller Online!" << std::endl;
	CommandExecutor* CExe = new CommandExecutor(running, Commandqueue);
	std::thread ExecutorThread = std::thread(&CommandExecutor::Execute, CExe);// thread that executes commands in commandqueue
	Intelligence* AI = new Intelligence(Datacollector, Commandqueue, VisionQueue, running, Worker, Sensor, VisionApi);
	std::thread AIThread = std::thread(&Intelligence::Think, AI);// thread that looks at current data, and generates new commands based on that

	CommandExecutor* VisionExecutor = new CommandExecutor(running, VisionQueue);
	VisionExecutor->Execute();//this thread executes vision commands

	ExecutorThread.join();
	AIThread.join();

    return 0;
}
