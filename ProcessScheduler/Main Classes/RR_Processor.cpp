#include "RR_Processor.h"

RR_Processor::RR_Processor(int ID, int TimeSlice) : Processor(ID), TimeSlice(TimeSlice)
{
}

void RR_Processor::ScheduleAlgo() 
{
	//assuming this function is called by the simulator each (timeslice) timesteps

	if (RunPtr)							//in case there is a process already executing
	{
		if (!RunPtr->GetCPUTime())		//in case the running process is not done executing
		{ //The process should go back to RDY, and another one comes in its place

			ReadyQ.Enqueue(*RunPtr);
		}
		else
		{ //process is done executing, move to TRM list
			//move
		}
	}

	//these 2 lines will be executed in every case
	*RunPtr = ReadyQ.Queue_front();
	RunPtr->ChangeProcessState(RUN);
	ReadyQ.Dequeue();

	if (RunPtr)				//if there is a process currently running
		CrntState = BUSY;
	else
		CrntState = IDLE;
}
