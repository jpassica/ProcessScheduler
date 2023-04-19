#pragma once
#include "Processor.h"

class RR_Processor : public Processor
{
	int TimeSlice;
	Queue<Process*> RR_Ready;

public:
	RR_Processor(int ID, int TimeSlice, scheduler*);

	//Picks the next process to run according to whose turn  it is in the queue
	virtual void ScheduleAlgo() override;

	//calculates and returns finish time according to CT of processes
	virtual int CalcFinishTime() override;


	Queue<Process*>& getRDY();
};

