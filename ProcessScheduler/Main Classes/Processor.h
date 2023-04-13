#pragma once
#include "Process.h"
//#include 

enum ProcessorState { IDLE, BUSY };

class Processor
{
private:
	int ID;
	int busyTime, idleTime;
	int finishTime;
	
protected:
	//Ptr to the running process
	Process* RunPtr;								
	//Scheduler* pScheduler;
	ProcessorState CrntState;

public:
	//ctor receives ID from the scheduler
	Processor(int ID);

	//
	virtual void ScheduleAlgo() = 0;

	//calculates and returns pLoad %
	double CalcPLoad(int TotalTRT);			

	//calculates and returns pUtil %
	double CalcPUtil() const;			

	//To be used for simulation each time step
	bool DecrementRunningProcess();		

	//This function should be called each time step, to track the time accurately
	void IncrementBusyAndIdleTime();

	//changes current state from idle to busy and vice versa
	void FlipProcessorState();
};