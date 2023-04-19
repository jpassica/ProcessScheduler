#include "SJF_Processor.h"

SJF_Processor::SJF_Processor(int ID , Scheduler* pSch) : Processor(ID , pSch)
{}

void SJF_Processor::ScheduleAlgo()
{ 
}

int SJF_Processor::CalcFinishTime()
{
	return 0;
}

void SJF_Processor::AddToReadyQueue(Process* pReady)
{
	SJF_Ready.Enqueue(pReady, pReady->GetCPUTime());
}

bool SJF_Processor::isReadyQueueEmpty() const
{
	if (SJF_Ready.isEmpty())
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

	SJF_Ready.QueueFront(newRunPtr);

	if (newRunPtr->isRecentlyUpdated(crntTimeStep))
		return false;

	SJF_Ready.Dequeue(RunPtr);

	CrntState = BUSY;
	RunPtr->SetLastUpdateTime(crntTimeStep);
	RunPtr->ChangeProcessState(RUN);

	return true;
}

int SJF_Processor::GetRDYCount() const
{
	return SJF_Ready.getCount();
}

void SJF_Processor::printRDY() const
{
	SJF_Ready.Print();
}
