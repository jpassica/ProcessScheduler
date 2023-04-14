#include "RR_Processor.h"

RR_Processor::RR_Processor(int ID, int TimeSlice) : Processor(ID), TimeSlice(TimeSlice)
{
}

void RR_Processor::ScheduleAlgo() 
{
	if (RunPtr)					//in case there is a process already executing
	{
		if (!RunPtr->GetCPUTime())		//in case the running process is not done executing
		{
			cerr << "Wait until the process finishes executing.\n";
		}
		else
		{ //process is done executing, move to TRM list



		}
	}

}

Queue<Process> RR_Processor::getRDY()
{
	return ReadyQ;
}
