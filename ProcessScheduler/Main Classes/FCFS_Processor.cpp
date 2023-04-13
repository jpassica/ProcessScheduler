#include "FCFS_Processor.h"

FCFS_Processor::FCFS_Processor(int ID) : Processor(ID)
{
}

void FCFS_Processor::ScheduleAlgo()
{
	if (ReadyQ.isEmpty())
		cerr << "No process to run.\n";
	else
	{
		if (!RunPtr->GetCPUTime())
		{ //process is done executing, it needs to be moved to TRM
			//pScheduler should move it to TRM
			*RunPtr = ReadyQ.Queue_front();
			bool DQ = ReadyQ.Dequeue();
			if (!DQ)
			{
				cerr << "No process to run.\n";
			}
			else
			{ //Now RUN points to new 
				RunPtr->ChangeProcessState(RUN);
		
			}

		}
	}

	//assuming this function is only called when necessary, 
	//when the running process is done executing

	//1: move done process to TRM list
	//move

	//2: place next process (if exists) in RUN state
		

	if (RunPtr)				//if there is a process currently running
		CrntState = BUSY;
	else
		CrntState = IDLE;
}
