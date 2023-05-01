#include "EDF_Processor.h"

EDF_Processor::EDF_Processor(int ID, Scheduler* SchedulerPtr) : Processor(ID, SchedulerPtr)
{
}

void EDF_Processor::ScheduleAlgo(int)
{
}

void EDF_Processor::AddToReadyQueue(Process* pReady)
{
	EDF_Ready.insert(EDF_Ready.getCount() + 1, pReady);

	FinishTime += pReady->GetRemainingCPUTime();
}

bool EDF_Processor::isReadyQueueEmpty() const
{
	if (EDF_Ready.isEmpty())
		return true;
	else
		return false;
}

bool EDF_Processor::fromReadyToRun(int crntTimeStep)
{
	if (RunPtr || CrntState == BUSY)
		return false;

	if (isReadyQueueEmpty())
		return false;

	Process* newRunPtr = EDF_Ready.getEntry(1);


	RunPtr = newRunPtr;
	EDF_Ready.remove(1);

	CrntState = BUSY;
	RunPtr->ChangeProcessState(RUN);

	if (RunPtr->isFirstExecution())
		RunPtr->SetResponseTime(crntTimeStep);

	FinishTime -= RunPtr->GetRemainingCPUTime();

	return true;
}

int EDF_Processor::GetRDYCount() const
{
	return EDF_Ready.getCount();
}

void EDF_Processor::printRDY() const
{
	EDF_Ready.Print();
}

Process* EDF_Processor::StealProcess()
{
	Process* StolenProcess = EDF_Ready.getEntry(1);

	EDF_Ready.remove(1);

	FinishTime -= StolenProcess->GetRemainingCPUTime();

	return StolenProcess;
}
