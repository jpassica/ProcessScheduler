#pragma once
#include "Processor.h"
#include "..\DS Classes\Queue.h"

class FCFS_Processor : public Processor
{
	Queue<Process> RDY;

public:
	FCFS_Processor(int ID);
	virtual void ScheduleAlgo() override;
};

