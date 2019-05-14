#include <iostream>
#include <string>
#include <queue>
#include "Command.h"
#include "Arduino.h" 
#include "GuardedQueue.h"
#include <chrono>
#include <thread>
#include "CommandExecutor.h"


bool running = true;
GuardedQueue<Command>* Commandqueue = new GuardedQueue<Command>();
Arduino* Worker = new Arduino("/dev/ttyACM0");
Arduino* Sensor = new Arduino("/dev/ttyACM1");
void loop() {
	while (running == true) {

		//running = false;

		Commandqueue->push(Command(Sensor, "refresh?"));
		std::string SensorResponce = Sensor->GetLastResponce();
		if (Commandqueue->GetSize() > 10) {
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			//std::cout << "Queue to large!" << std::endl;
		}
		std::cout << Sensor->GetLastResponce();
		/*if (SensorResponce == "On")
		{
			Commandqueue->push(Command(Worker, "LedOn"));
		}
		if (SensorResponce == "Off") {
			Commandqueue->push(Command(Worker, "LedOff"));
		}*/
	}
}

int main()
{	
	std::cout << "Controller Online!" << std::endl;
	CommandExecutor* CExe = new CommandExecutor(running, Commandqueue);
	std::thread ExecutorThread = std::thread(&CommandExecutor::Execute, CExe);
	loop();

    return 0;
}

