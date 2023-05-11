#ifndef RR_PROCESSOR_H
#define RR_PROCESSOR_H

#include "Processor.h"

class RR_Processor : public Processor
{
	int TimeSlice;
	int TimeSliceCounter;
	Queue<Process*> RR_Ready;

public:
	RR_Processor(int ID, int timeSlice, Scheduler* SchedulerPtr, int HealingTime);

	//Handles moving processes to and from RUN state
	virtual void ScheduleAlgo(int) override;

	//adds passed process to RR_Ready
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