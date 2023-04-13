#pragma once
#include "Processor.h"
#include "..\DS Classes\PriorityQueue.h"

class SJF_Processor : public Processor
{
	PriorityQueue<Process> ReadyPriQ;

public:
	SJF_Processor(int ID);
	virtual void ScheduleAlgo() override;
};

