#pragma once

class Processor
{
	const int ID;

public:
	Processor();
	virtual void ScheduleAlgo() = 0;
};