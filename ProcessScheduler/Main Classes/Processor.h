#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <iostream>
#include "Process.h"
using namespace std;

class Scheduler;

class Processor
{
private:
	int ID;
	int BusyTime, IdleTime;
	
protected:
	Process* RunPtr;						//ptr to the running process
	Scheduler* pScheduler;					//ptr to Scheduler class
	ProcessorState CrntState;				//current state idle vs. busy
	int FinishTime;							//estimated finish time of all processes for the processor

public:
	//ctor receives ID from the scheduler
	Processor(int ID, Scheduler* SchedulerPtr);

	//each processor has its own version of this function
	virtual void ScheduleAlgo(int) = 0;

	//outputs Processor's ID
	friend ostream& operator<<(ostream&, const Processor&);  

	//calculates and returns pLoad %
	double CalcPLoad(int TotalTRT) const;			

	//calculates and returns pUtil %
	double CalcPUtil() const;			

	//Increments the ProcessedTime of the running process
	//it should be called each time step
	void IncrementRunningProcess();		

	//This function keeps track of BusyTime and IdleTime
	//it should be called each time step
	void IncrementBusyOrIdleTime();

	//changes current state from idle to busy and vice versa
	void FlipProcessorState();

	//returns processor ID
	int getID() const;

	//sets runnning processo to a passed process
	void setRunptr(Process*);

	//returns crnt processor state
	ProcessorState GetProcessorState();

	//returns run ptr
	Process* GetRunPtr();

	//this function is overriden in each processor class
	virtual void AddToReadyQueue(Process* pReady) = 0;

	//returns true if ready queue/list is empty
	virtual bool isReadyQueueEmpty() const = 0;

	//moves process at top of ready queue/list to run
	virtual bool fromReadyToRun(int crntTimeStep) = 0;

	//returns count of items in ready queue/list
	virtual int GetRDYCount() const = 0;

	//returns expected finish time of all processes for the processor
	virtual int GetFinishTime() const = 0;

	//Calls print function of ready queue/list
	virtual void printRDY() const = 0;
	
	//Steals a process from the top of the RDY queue/list
	virtual Process* StealProcess() = 0;
};

#endif