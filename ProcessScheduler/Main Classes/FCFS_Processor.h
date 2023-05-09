#ifndef FCFS_PROCESSOR_H
#define FCFS_PROCESSOR_H

#include "Processor.h"
#include "..\DS Classes\ProcessList.h"

class FCFS_Processor : public Processor
{
	ProcessList FCFS_Ready;
	static Queue<KillSignal*> KillSignalQ;
	int ForkProbability;

public:
	FCFS_Processor(int, int, Scheduler*);

	//Handles moving processes to and from RUN state
	virtual void ScheduleAlgo(int) override;

	//adds the passed process to the FCFS_Ready
	virtual void AddToReadyQueue(Process*) override;

	virtual bool isReadyQueueEmpty() const override;

	virtual bool RunNextProcess(int) override;

	virtual int GetRDYCount() const override;

	bool KillByID(int);

	virtual void PrintRDY() const override;

	virtual Process* StealProcess() override;

	//Kills processes if they receive a kill signal at current Time step
	bool ExecuteKIllSignal();

	bool KillOrphan(int);

	//Creates KillSignal object and adds it to the kill signal queue
	static void AddKillSignal(int, int);

	//Called at end of program to deallocate any remaining unexecuted kill signals
	static void ClearKillSignalQ();
};

#endif