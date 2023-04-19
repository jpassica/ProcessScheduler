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

int SJF_Processor::GetRDYCount() const
{
	return ReadyPriQ.getCount();
}

void SJF_Processor::printRDY() const
{
	ReadyPriQ.Print();
}
