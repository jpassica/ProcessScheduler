#include "RR_Processor.h"

RR_Processor::RR_Processor(int ID, int TimeSlice , scheduler* pShc) : Processor(ID , pShc), TimeSlice(TimeSlice)
{}

void RR_Processor::ScheduleAlgo() 
{
	//assuming this function is called by the simulator each (timeslice) timesteps

	if (RunPtr)							//in case there is a process already executing
	{
		if (RunPtr->GetCPUTime())		//in case the running process is not done executing
		{ //The process should go back to RDY, and another one comes in its place

			RR_Ready.Enqueue(RunPtr);
		}
		else
		{ //process is done executing, move to TRM list
			//move
		}
	}

	if (!RR_Ready.isEmpty())
	{
		Process* temp;
		RunPtr = RR_Ready.Queue_front();
		RunPtr->ChangeProcessState(RUN);
		RR_Ready.Dequeue(temp);
	}
	else
	{
		RunPtr = nullptr;
	}

	if (RunPtr)				//if there is a process currently running
		CrntState = BUSY;
	else
		CrntState = IDLE;
}

int RR_Processor::CalcFinishTime()
{
	return 0;
}

Queue<Process*>& RR_Processor::getRDY()
{
	return RR_Ready;
}
