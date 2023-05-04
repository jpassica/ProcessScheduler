#ifndef FCFS_PROCESSOR_H
#define FCFS_PROCESSOR_H

#include "Processor.h"
#include "..\DS Classes\ProcessList.h"

class FCFS_Processor : public Processor
{
	ProcessList FCFS_Ready;

public:
	FCFS_Processor(int ID, Scheduler* SchedulerPtr);

	//Handles moving processes to and from RUN state
	virtual void ScheduleAlgo(int) override;

	//adds the passed process to the FCFS_Ready
	virtual void AddToReadyQueue(Process* pReady) override;

	virtual bool isReadyQueueEmpty() const override;

	virtual bool RunNextProcess(int crntTimeStep) override;

	virtual int GetRDYCount() const override;

	bool KillByID(int randomID);

	virtual void PrintRDY() const override;

	virtual Process* StealProcess() override;

	//Kills processes if they receive a kill signal at current time step
	void Kill();
	bool SearchProcess(int) const;

};

#endif