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
		
}
