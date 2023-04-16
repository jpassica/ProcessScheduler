#pragma once
#include "Processor.h"
#include "..\DS Classes\PriorityQueue.h"

class SJF_Processor : public Processor
{
	PriorityQueue<Process*> ReadyPriQ;

public:
	SJF_Processor(int ID);

	//Picks the next process to run according to 'shortest job first'
	virtual void ScheduleAlgo() override;

	//calculates and returns finish time according to CT of processes
	virtual int CalcFinishTime() override;

	PriorityQueue<Process*>& getRDY();
};

