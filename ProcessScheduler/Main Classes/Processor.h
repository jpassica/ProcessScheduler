#pragma once
#include "Process.h"
//#include 

enum ProcessorState { IDLE, BUSY };

class Processor
{
private:
	int ID;
	ProcessorState CrntState;
	int busyTime, idleTime;
	int finishTime;
	
protected:
	Process* RunPtr;								//Ptr to the executing process
	//Scheduler* pScheduler;

public:
	Processor(int ID);								//ctor receives ID from the scheduler
	virtual void ScheduleAlgo() = 0;
	double CalcPLoad(int TotalTRT);					//calculates Processor Load and re
	double CalcPUtil() const;
};