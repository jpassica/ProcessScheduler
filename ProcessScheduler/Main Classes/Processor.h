#pragma once
#include "Process.h"
//#include 

enum ProcessorState { IDLE, BUSY };

class Processor
{
private:
	int ID;
	ProcessorState CrntState;
	//double pLoad, pUtil;							//not really using those eh?
	int busyTime, idleTime;
	int finishTime;
	
protected:
	Process* RUN;

public:
	Processor(int ID);								//ctor receives ID from the scheduler
	virtual void ScheduleAlgo() = 0;
	double CalcProcessorLoad(int TotalTRT);			//calculates Processor Load and re
	double CalcProcessorUtil() const;
};