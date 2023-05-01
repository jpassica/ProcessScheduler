#include "FCFS_Processor.h"
#include "Scheduler.h"

FCFS_Processor::FCFS_Processor(int ID, Scheduler* SchedulerPtr) : Processor(ID, SchedulerPtr) {}

void FCFS_Processor::ScheduleAlgo(int CrntTimeStep)
{
}

void FCFS_Processor::AddToReadyQueue(Process* pReady)
{
	FCFS_Ready.insert(FCFS_Ready.getCount() + 1, pReady);

	FinishTime += pReady->GetRemainingCPUTime();
}

bool FCFS_Processor::isReadyQueueEmpty() const
{
	if (FCFS_Ready.isEmpty())
		return true;
	else
		return false;
}

bool FCFS_Processor::fromReadyToRun(int crntTimeStep)
{
	if (RunPtr || CrntState == BUSY)
		return false;

	if (isReadyQueueEmpty())
		return false;

	Process* newRunPtr = FCFS_Ready.getEntry(1);


	RunPtr = newRunPtr;
	FCFS_Ready.remove(1);

	CrntState = BUSY;
	RunPtr->ChangeProcessState(RUN);

	if (RunPtr->isFirstExecution())
		RunPtr->SetResponseTime(crntTimeStep);

	FinishTime -= RunPtr->GetRemainingCPUTime();

	return true;
}

int FCFS_Processor::GetRDYCount() const
{
	return FCFS_Ready.getCount();
}

bool FCFS_Processor::RandomKill(int randomID)
{
	int position = FCFS_Ready.SearchByID(randomID);

	if (position)
	{
		Process* killedProcess = FCFS_Ready.getEntry(position);

		if (pScheduler->TerminateProcess(killedProcess))
		{
			FCFS_Ready.remove(position);
			
			FinishTime -= killedProcess->GetRemainingCPUTime();


			return true;
		}
		else
			return false;
	}
	else
		return false;
}

void FCFS_Processor::printRDY() const
{
	FCFS_Ready.Print();
}

Process* FCFS_Processor::StealProcess()
{
	Process* StolenProcess = FCFS_Ready.getEntry(1);

	FCFS_Ready.remove(1);

	FinishTime -= StolenProcess->GetRemainingCPUTime();

	return StolenProcess;
}
