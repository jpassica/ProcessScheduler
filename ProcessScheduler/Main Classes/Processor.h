#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "Process.h"
using namespace std;

class Scheduler;

class Processor
{
private:
	int ID;
	int BusyTime, IdleTime;
	
protected:
	Process* RunPtr;						//Ptr to the running process
	Scheduler* pScheduler;					//Ptr to Scheduler class
	ProcessorState CrntState;				//Current state: idle vs. busy
	int FinishTime;							//Estimated finish time of all processes in the ready queue/list

public:
	Processor(int ID, Scheduler* SchedulerPtr);

	//Handles moving processes to and from RUN state
	virtual void ScheduleAlgo(int) = 0;

	//Outputs Processor's ID
	friend ostream& operator<<(ostream&, const Processor&);  

	//Calculates and returns pLoad %
	double CalcPLoad(int TotalTRT) const;			

	//Calculates and returns pUtil %
	double CalcPUtil() const;			

	//Increments the ProcessedTime of the running process
	//it should be called each time step
	void IncrementRunningProcess();		

	//Keeps track of BusyTime and IdleTime
	//it should be called each time step
	void IncrementBusyOrIdleTime();

	//Changes current state from idle to busy and vice versa
	void ChangeProcessorState(ProcessorState NextState);

	//Returns processor ID
	int getID() const;

	//sets runnning processo to a passed process
	void SetRunptr(Process*);

	//Returns crnt processor state
	ProcessorState GetProcessorState();

	//Returns run ptr
	Process* GetRunPtr();

	//this function is overriden in each processor class
	virtual void AddToReadyQueue(Process* pReady) = 0;

	//returns true if ready queue/list is empty
	virtual bool isReadyQueueEmpty() const = 0;

	//moves process at top of ready queue/list to run
	virtual bool RunNextProcess(int crntTimeStep) = 0;

	//returns count of items in ready queue/list
	virtual int GetRDYCount() const = 0;

	//returns expected finish time of all processes in the ready queue/list
	int GetFinishTime() const;

	//Calls print function of ready queue/list
	virtual void PrintRDY() const = 0;
	
	//Steals a process from the top of the RDY queue/list
	virtual Process* StealProcess() = 0;
};

#endif