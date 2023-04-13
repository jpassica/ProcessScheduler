#include "SJF_Processor.h"

SJF_Processor::SJF_Processor(int ID) : Processor(ID)
{
}

void SJF_Processor::ScheduleAlgo()
{ 
	//assuming this function will be called only when needed,
    //meaning it will only be called when the running process has finished executing

	if (!RunPtr->GetCPUTime())
	{ 
		if (!dq)
		{
			
		}
		else
		{ // Now RunPtr points to the new process
			RunPtr->ChangeProcessState(RUN);

		}
	}
	else
	{
		cerr << "Error! Wait until the process finishes executing.\n";
	}

	//1: move process to TRM list
	//move

	//2: point to new process and dequeue
	int x(0);
	bool CanPeek = ReadyPriQ.QueueFront(*RunPtr, x);

	if (!CanPeek)
	{   //couldn't peek because ReadyPriQ is empty
	    //no need to dequeue
		RunPtr = nullptr;
		//processor is now idle, it has no process to execute
	}
	bool dq = ReadyPriQ.Dequeue();

	if (!dq)
	{ //couldn't dequeue because ReadyPriQ is empty
		RunPtr = nullptr;
		//processor is now idle, it has no process to run.
	}

	if (RunPtr)				//if there is a process currently running
		CrntState = BUSY;
	else
		CrntState = IDLE;
}
