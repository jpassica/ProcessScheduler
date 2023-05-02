#ifndef FCFS_PROCESSOR_H
#define FCFS_PROCESSOR_H

#include "Processor.h"
#include "..\DS Classes\ListProcessPtrs.h"

class FCFS_Processor : public Processor
{
	ListProcessPtrs FCFS_Ready;

public:
	FCFS_Processor(int ID, Scheduler* SchedulerPtr);

	//Picks the next process to run according to 'first come first serve'
	virtual void ScheduleAlgo(int) override;

	//adds the passed process to the FCFS_Ready
	virtual void AddToReadyQueue(Process* pReady) override;

	virtual bool isReadyQueueEmpty() const override;

	virtual bool fromReadyToRun(int crntTimeStep) override;

	virtual int GetRDYCount() const override;

	bool RandomKill(int randomID);
	bool KillById(int ID);

	virtual void printRDY() const override;

	virtual Process* StealProcess() override;
};

#endif