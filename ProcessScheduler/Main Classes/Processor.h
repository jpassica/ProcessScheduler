#pragma once
#include <iostream>
#include "Process.h"
using namespace std;

class Scheduler;

class Processor
{
private:
	int ID;
	int busyTime, idleTime;
	
protected:
	Process* RunPtr;						//ptr to the running process
	Scheduler* pScheduler;					//ptr to Scheduler class
	ProcessorState CrntState;				//current state idle vs. busy
	int finishTime;							//estimated finish time of all processes for the processor

public:
	//ctor receives ID from the scheduler
	Processor(int ID , Scheduler*);

	//each processor has its own version of this function
	virtual void ScheduleAlgo() = 0;

	//outputs Processor's ID
	friend ostream& operator<<(ostream&, const Processor&);  
	
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

	//returns processor ID
	int getID() const;

	//sets runnning processo to a passed process
	void setRunptr(Process*);

	//returns crnt processor state
	ProcessorState getProcessorState();

	//returns run ptr
	Process* getRunPtr();

	//this function is overriden in each processor class
	virtual void AddToReadyQueue(Process* pReady) = 0;

	//returns true if ready queue/list is empty
	virtual bool isReadyQueueEmpty() const = 0;

	//moves process at top of ready queue/list to run
	virtual bool fromReadyToRun(int crntTimeStep) = 0;

	//returns count of items in ready queue/list
	virtual int GetRDYCount() const = 0;

	//calls print function of ready queue/list
	virtual void printRDY() const = 0;
};