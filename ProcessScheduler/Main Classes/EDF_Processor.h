#pragma once
#include "Processor.h"
#include "..\DS Classes\PriorityQueue.h"

class EDF_Processor : public Processor
{
	PriorityQueue<Process*> EDF_Ready;

public:
	EDF_Processor(int ID, Scheduler* SchedulerPtr);

	virtual void ScheduleAlgo(int) override;

	//this function is overriden in each processor class
	virtual void AddToReadyQueue(Process* pReady) override;

	//returns true if ready queue/list is empty
	virtual bool isReadyQueueEmpty() const override;

	//moves process at top of ready queue/list to run
	virtual bool RunNextProcess(int crntTimeStep) override;

	//returns count of items in ready queue/list
	virtual int GetRDYCount() const override;

	//Calls print function of ready queue/list
	virtual void PrintRDY() const override;

	//Steals a process from the top of the RDY queue/list
	virtual Process* StealProcess() override;

};

