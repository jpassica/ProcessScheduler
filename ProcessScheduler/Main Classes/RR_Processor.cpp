#include "RR_Processor.h"

RR_Processor::RR_Processor(int ID, int TimeSlice , scheduler* pShc) : Processor(ID , pShc), TimeSlice(TimeSlice)
{}

void RR_Processor::ScheduleAlgo() 
{
}

int RR_Processor::CalcFinishTime()
{
	return 0;
}

void RR_Processor::AddToReadyQueue(Process* pReady)
{
	ReadyQ.Enqueue(pReady);
}

bool RR_Processor::isReadyQueueEmpty() const
{
	if (ReadyQ.isEmpty())
		return true;
	else
		return false;
}

bool RR_Processor::fromReadyToRun(int crntTimeStep)
{
	if (RunPtr || CrntState == BUSY)
		return false;

	if (isReadyQueueEmpty())
		return false;

	Process* newRunPtr = ReadyQ.Queue_front();

	if (newRunPtr->isRecentlyUpdated(crntTimeStep))
		return false;

	ReadyQ.Dequeue(RunPtr);

	CrntState = BUSY;
	RunPtr->SetLastUpdateTime(crntTimeStep);
	RunPtr->ChangeProcessState(RUN);

	return true;
}

Queue<Process*>& RR_Processor::getRDY()
{
	return ReadyQ;
}
