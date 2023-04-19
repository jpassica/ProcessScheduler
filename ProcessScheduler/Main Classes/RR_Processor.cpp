#include "RR_Processor.h"

RR_Processor::RR_Processor(int ID, int TimeSlice , Scheduler* pShc) : Processor(ID , pShc), TimeSlice(TimeSlice)
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
	RR_Ready.Enqueue(pReady);
}

bool RR_Processor::isReadyQueueEmpty() const
{
	if (RR_Ready.isEmpty())
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

	Process* newRunPtr = RR_Ready.Queue_front();

	if (newRunPtr->isRecentlyUpdated(crntTimeStep))
		return false;

	RR_Ready.Dequeue(RunPtr);

	CrntState = BUSY;
	RunPtr->SetLastUpdateTime(crntTimeStep);
	RunPtr->ChangeProcessState(RUN);

	return true;
}

int RR_Processor::GetRDYCount() const
{
	return RR_Ready.getCount();
}

void RR_Processor::printRDY() const
{
	RR_Ready.Print();
}
