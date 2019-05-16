#pragma once
#include "DataCollector.h"
#include "GuardedQueue.h"
#include "Arduino.h"
#include <thread>
#include <chrono>
#include <utility>
class Intelligence
{
public:
	Intelligence(DataCollector* DC, GuardedQueue<Command>* GQ , bool* RN, Arduino* WKR, Arduino*SNR );
	~Intelligence();
	void Think();

private:
	DataCollector* Database;
	GuardedQueue<Command>* CommandQueue;
	bool* running;
	Arduino* Worker;
	Arduino* Sensor;
};

