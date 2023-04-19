#include "SJF_Processor.h"

SJF_Processor::SJF_Processor(int ID , scheduler* pSch) : Processor(ID , pSch)
{}

void SJF_Processor::ScheduleAlgo()
{ 
	//assuming this function will be called only when needed,
    //meaning it will only be called when the running process has finished executing

	//1: move process to TRM list
	//move

	//2: point to new process and dequeue
	int key(0);
	bool CanPeek = SJF_Ready.QueueFront(RunPtr);

	if (!CanPeek)
	{   //ReadyPriQ is empty, no need to dequeue
		RunPtr = nullptr;
		//processor is now idle, it has no process to execute
	}
	else
	{   //now RunPtr to new running process
		Process* s;
		bool dq = SJF_Ready.Dequeue(s);
		RunPtr->ChangeProcessState(RUN);
	}

	if (RunPtr)				//if there is a process currently running
		CrntState = BUSY;
	else
		CrntState = IDLE;
}

int SJF_Processor::CalcFinishTime()
{
	return 0;
}

PriorityQueue<Process*>& SJF_Processor::getRDY()
{
	return SJF_Ready;
}
