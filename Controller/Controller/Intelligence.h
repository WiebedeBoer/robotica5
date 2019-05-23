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
	Intelligence(DataCollector* DC, GuardedQueue<Command>* GQ , GuardedQueue<Command>* VQ, bool* RN, MicroController* WKR, MicroController*SNR, Vision* V);
	~Intelligence();
	void Think();

private:
	DataCollector* Database;
	GuardedQueue<Command>* CommandQueue;
	GuardedQueue<Command>* VisionQueue;
	bool* running;
	MicroController* Worker;
	MicroController* Sensor;
	Vision* VisionApi;
};

