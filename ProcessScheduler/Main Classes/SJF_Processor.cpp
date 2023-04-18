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
	bool CanPeek = ReadyPriQ.QueueFront(RunPtr);

	if (!CanPeek)
	{   //ReadyPriQ is empty, no need to dequeue
		RunPtr = nullptr;
		//processor is now idle, it has no process to execute
	}
	else
	{   //now RunPtr to new running process
		Process* s;
		bool dq = ReadyPriQ.Dequeue(s);
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

void SJF_Processor::AddToReadyQueue(Process* pReady)
{
	ReadyPriQ.Enqueue(pReady, pReady->GetCPUTime());
}

bool SJF_Processor::isReadyQueueEmpty() const
{
	if (ReadyPriQ.isEmpty())
		return true;
	else
		return false;
}

bool SJF_Processor::fromReadyToRun(int crntTimeStep)
{
	if (RunPtr || CrntState == BUSY)
		return false;

	if (isReadyQueueEmpty())
		return false;

	Process* newRunPtr(nullptr);

	ReadyPriQ.QueueFront(newRunPtr);

	if (newRunPtr->isRecentlyUpdated(crntTimeStep))
		return false;

	ReadyPriQ.Dequeue(RunPtr);

	CrntState = BUSY;
	RunPtr->SetLastUpdateTime(crntTimeStep);
	RunPtr->ChangeProcessState(RUN);

	return true;
}

PriorityQueue<Process*>& SJF_Processor::getRDY()
{
	return ReadyPriQ;
}
