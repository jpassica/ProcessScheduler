#pragma once
#include "Processor.h"
#include "..\DS Classes\Queue.h"

class FCFS_Processor : public Processor
{
	Queue<Process> ReadyQ;

public:
	FCFS_Processor(int ID);
	virtual void ScheduleAlgo() override;
	Queue<Process> getRDY();
};

