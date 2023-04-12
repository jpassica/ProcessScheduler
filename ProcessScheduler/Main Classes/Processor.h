#pragma once

enum ProcessorState {IDLE, BUSY};

class Processor
{
	int ID;
	ProcessorState CrntState;
	double pLoad, pUtil;
	int busyTime, idleTime;
	int finishTime;

public:
	Processor(int ID);					//ctor receives ID from the scheduler
	virtual void ScheduleAlgo() = 0;
};