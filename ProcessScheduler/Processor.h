#pragma once

enum ProcessorState {IDLE, BUSY};

class Processor
{
	const int ID;
	ProcessorState CrntState;

public:
	Processor();
	virtual void ScheduleAlgo() = 0;
};