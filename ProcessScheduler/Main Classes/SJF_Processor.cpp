#include "SJF_Processor.h"

SJF_Processor::SJF_Processor(int ID) : Processor(ID)
{
}

void SJF_Processor::ScheduleAlgo()
{
	if (!RunPtr->GetCPUTime())
	{ //process is done executing
		//move process to TRM
		int x(0);
		bool peek = ReadyPriQ.QueueFront(*RunPtr, x);
		bool dq = ReadyPriQ.Dequeue();
		if (!dq)
		{
			cerr << "lol idk\n";
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
}

const PriorityQueue<Process>& SJF_Processor::getRDY()
{
	return ReadyPriQ;
}
