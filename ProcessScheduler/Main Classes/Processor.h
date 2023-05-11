#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "Process.h"

// Forward declaration  
class Scheduler;

class Processor
{
protected:
	int ID;
	ProcessorState CrntState;				//idle vs. busy
	int BusyTime, IdleTime;					//Total Time the processor was busy/idle
	Process* RunPtr;						//Ptr to the running process
	Scheduler* pScheduler;					//Ptr to Scheduler class
	int FinishTime;							//Estimated finish Time of all processes in the ready queue/list
	int HealingTime;                        //time needed for healing   
	int HealingTimeCounter;

public:
	Processor(int ID, Scheduler* SchedulerPtr, int Healing);

	//Handles moving processes to and from RUN state
	virtual void ScheduleAlgo(int) = 0;

	//this function is overriden in each processor class
	virtual void AddToReadyQueue(Process* pReady) = 0;

	//Returns true if ready queue/list is empty
	virtual bool isReadyQueueEmpty() const = 0;

	//Moves process at top of ready queue/list to run
	virtual bool RunNextProcess(int crntTimeStep) = 0;

	//Returns count of items in ready queue/list
	virtual int GetRDYCount() const = 0;

	//Calls print function of ready queue/list
	virtual void PrintRDY() const = 0;

	//Steals a process from the top of the RDY queue/list
	virtual Process* StealProcess() = 0;

	//Stops the processor and moves its RUN/RDY prosesses to shortest ready queue
	virtual void ClearOverheatedProcessor() = 0;

	//Outputs Processor's ID
	friend ostream& operator<<(ostream&, const Processor&);  

	//Calculates and returns pLoad %
	double CalcPLoad(int TotalTRT) const;			

	//Calculates and returns pUtil %
	double CalcPUtil() const;				

	//Returns processor ID
	int GetID() const;

	//increments BusyTime || IdleTime based on current state
	void IncrementBusyOrIdleTime();

	//Returns true if the processor is currently running a process
	bool isExecutingProcess() const;

	//Returns ID of the currently running process
	int GetRunningProcessID() const;

	//returns expected finish Time of all processes in the ready queue/list
	int GetFinishTime() const;

	//returns if the processor is stopped
	bool IsStopped() const;

	//reduce the healing time
	void ContinueHealing();
};

#endif