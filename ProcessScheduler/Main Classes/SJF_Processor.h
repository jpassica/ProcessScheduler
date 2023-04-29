#ifndef SJF_PROCESSOR_H
#define SJF_PROCESSOR_H

#include "Processor.h"
#include "..\DS Classes\PriorityQueue.h"

class SJF_Processor : public Processor
{
	PriorityQueue<Process*> SJF_Ready;

public:
	SJF_Processor(int ID, Scheduler* SchedulerPtr);

	//Picks the next process to run according to 'shortest job first'
	virtual void ScheduleAlgo(int) override;

	//adds passed process to SJF_Ready
	virtual void AddToReadyQueue(Process* pReady) override;

	virtual bool isReadyQueueEmpty() const override;

	virtual bool fromReadyToRun(int crntTimeStep) override;

	virtual int GetRDYCount() const override;

	virtual int GetFinishTime() const override;

	virtual void printRDY() const override;
};

#endif