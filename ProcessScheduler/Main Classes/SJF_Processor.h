#ifndef SJF_PROCESSOR_H
#define SJF_PROCESSOR_H

#include "Processor.h"
#include "..\DS Classes\PriorityQueue.h"

class SJF_Processor : public Processor
{
	PriorityQueue<Process*> SJF_Ready;

public:
	SJF_Processor(int ID, Scheduler* SchedulerPtr, int HealingTime);

	//Handles moving processes to and from RUN state
	virtual void ScheduleAlgo(int) override;

	//adds passed process to SJF_Ready
	virtual void AddToReadyQueue(Process* pReady) override;

	//Returns true if ready queue/list is empty
	virtual bool isReadyQueueEmpty() const override;

	//Stops the processor and moves its RUN/RDY prosesses to shortest ready queue
	virtual void ClearOverheatedProcessor() override;

	//Moves process at top of ready queue/list to run
	virtual bool RunNextProcess(int crntTimeStep) override;

	//Returns count of items in ready queue/list
	virtual int GetRDYCount() const override;

	//Calls print function of ready queue/list
	virtual void PrintRDY() const override;

	//Steals process from the top of the ready queue
	virtual Process* StealProcess() override;
};

#endif