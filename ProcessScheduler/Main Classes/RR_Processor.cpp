#include "RR_Processor.h"

RR_Processor::RR_Processor(int ID, int timeSlice, Scheduler* SchedulerPtr) : Processor(ID, SchedulerPtr), timeSlice(timeSlice)
{}

void RR_Processor::ScheduleAlgo(int CrntTimeStep) 
{
}

void RR_Processor::AddToReadyQueue(Process* pReady)
{
	RR_Ready.Enqueue(pReady);

	FinishTime += pReady->GetCPUTime();
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

	Process* newRunPtr = RR_Ready.QueueFront();


	RR_Ready.Dequeue(RunPtr);

	CrntState = BUSY;
	RunPtr->ChangeProcessState(RUN);

	if (RunPtr->isFirstExecution())
		RunPtr->SetResponseTime(crntTimeStep);

	return true;
}

int RR_Processor::GetRDYCount() const
{
	return RR_Ready.getCount();
}

int RR_Processor::GetFinishTime() const
{
	return FinishTime;
}

void RR_Processor::printRDY() const
{
	RR_Ready.Print();
}

Process* RR_Processor::StealProcess()
{
	Process* StolenProcess = nullptr;

	RR_Ready.Dequeue(StolenProcess);

	FinishTime -= StolenProcess->GetCPUTime();

	return StolenProcess;
}
