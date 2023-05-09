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

	virtual bool isReadyQueueEmpty() const override;

	virtual void GoForHealing() override;

	virtual bool RunNextProcess(int crntTimeStep) override;

	virtual int GetRDYCount() const override;

	virtual void PrintRDY() const override;

	virtual Process* StealProcess() override;
};

#endif