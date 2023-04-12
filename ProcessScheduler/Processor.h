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
	Processor();
	virtual void ScheduleAlgo() = 0;
};