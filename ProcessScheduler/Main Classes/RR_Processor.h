#ifndef RR_PROCESSOR_H
#define RR_PROCESSOR_H

#include "Processor.h"

class RR_Processor : public Processor
{
	int TimeSlice;
	int TimeSliceCounter;
	Queue<Process*> RR_Ready;

public:
	RR_Processor(int ID, int timeSlice, Scheduler* SchedulerPtr);

	//Handles moving processes to and from RUN state
	virtual void ScheduleAlgo(int) override;

	//adds passed process to RR_Ready
	virtual void AddToReadyQueue(Process* pReady) override;

	virtual bool isReadyQueueEmpty() const override;

	virtual bool RunNextProcess(int crntTimeStep) override;
	
	virtual int GetRDYCount() const override;

	virtual void PrintRDY() const override;

	virtual Process* StealProcess() override;
};

#endif