#include <iostream>
#include <string>
#include <atomic>
#include <queue>
#include "Command.h"
#include "Arduino.h" 
#include "GuardedQueue.h"
#include <chrono>
#include <thread>
#include "CommandExecutor.h"
#include <memory>

bool running = true;
GuardedQueue<Command> Commandqueue;
Arduino Worker = Arduino("/dev/ttyACM0");
Arduino Sensor = Arduino("/dev/ttyACM1");
void loop() {
	while (running == true) {

		//running = false;

		Commandqueue.push(Command(Sensor, "Led?"));
		std::string SensorResponce = Sensor.GetLastResponce();
		std::this_thread::sleep_for(std::chrono::milliseconds(10000));

		if (SensorResponce == "On")
		{
			Commandqueue.push(Command(Worker, "LedOn"));
		}
		if (SensorResponce == "Off") {
			Commandqueue.push(Command(Worker, "LedOff"));
		}
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

