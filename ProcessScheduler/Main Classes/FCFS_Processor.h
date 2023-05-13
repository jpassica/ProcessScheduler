#ifndef FCFS_PROCESSOR_H
#define FCFS_PROCESSOR_H

#include "Processor.h"
#include "..\DS Classes\ProcessList.h"

class FCFS_Processor : public Processor
{
	ProcessList FCFS_Ready;
	static Queue<KillSignal*> KillSignalQ;			//Queue of kill signals shared by all FCFS processors
	int ForkProbability;

public:
	FCFS_Processor(int, int, Scheduler*, int HealingTime);

	//Handles moving processes to and from RUN state
	virtual void ScheduleAlgo(int) override;

	//adds the passed process to the FCFS_Ready
	virtual void AddToReadyQueue(Process*) override;

	//Returns true if ready queue/list is empty
	virtual bool isReadyQueueEmpty() const override;

	//Stops the processor and moves its RUN/RDY prosesses to shortest ready queue
	virtual void ClearOverheatedProcessor() override;

	//Moves process at top of ready queue/list to run
	virtual bool RunNextProcess(int) override;

	//Returns count of items in ready queue/list
	virtual int GetRDYCount() const override;

	//Kills the process with the passed ID if it found in RDY/RUN
	bool KillByID(int);

	//Calls print function of ready queue/list
	virtual void PrintRDY() const override;

	//Steals process from the top of the ready list
	virtual Process* StealProcess() override;

	//Kills processes if they receive a kill signal at current Time step
	bool ExecuteKillSignal();

	//Kills orphan process upon termination of its parent process
	bool KillOrphan(int);

	//Creates KillSignal object and adds it to the kill signal queue
	static void AddKillSignal(int, int);

	//Called at end of program to deallocate any remaining unexecuted kill signals
	static void ClearKillSignalQ();

	//Removes ignored kill signals at current time step from the queue
	static void RemoveIgnoredKillSignals(int);
};

#endif