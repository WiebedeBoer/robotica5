#pragma once
#include "DataCollector.h"
#include "GuardedQueue.h"
#include "MicroController.h"
#include <thread>
#include <chrono>
#include <utility>
class Intelligence
{
public:
	Intelligence(DataCollector* DC, GuardedQueue<Command>* GQ , bool* RN, MicroController* WKR, MicroController*SNR );
	~Intelligence();
	void Think();

private:
	DataCollector* Database;
	GuardedQueue<Command>* CommandQueue;
	bool* running;
	MicroController* Worker;
	MicroController* Sensor;
};

