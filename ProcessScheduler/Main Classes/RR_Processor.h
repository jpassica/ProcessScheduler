#pragma once
#include "Processor.h"
#include "..\DS Classes\Queue.h"

class RR_Processor : public Processor
{
	int TimeSlice;
	Queue<Process> ReadyQ;

public:
	RR_Processor(int ID, int TimeSlice);
	virtual void ScheduleAlgo() override;
};

