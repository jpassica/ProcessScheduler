#pragma once
#include <iostream>
#include "Process.h"
using namespace std;

class scheduler;

class Processor
{
private:
	int ID;
	int busyTime, idleTime;
	
protected:
	//Ptr to the running process
	Process* RunPtr;								
	scheduler* pScheduler;
	ProcessorState CrntState;
	int finishTime;

public:
	//ctor receives ID from the scheduler
	Processor(int ID);

	//each processor has its own version of this function
	friend ostream& operator<<(ostream&, const Processor&);  //outputs Processor's ID
	virtual void ScheduleAlgo() = 0;

	//calculates and returns finish time according to CT of processes
	virtual int CalcFinishTime() = 0;

	//calculates and returns pLoad %
	double CalcPLoad(int TotalTRT) const;			

	//calculates and returns pUtil %
	double CalcPUtil() const;			

	//This function decrements the CT of the running process
	//it should be called each time step
	bool DecrementRunningProcess();		

	//This function keeps track of busyTime and idleTime
	//it should be called each time step
	void IncrementBusyOrIdleTime();

	//changes current state from idle to busy and vice versa
	void FlipProcessorState();

	int getID();
};