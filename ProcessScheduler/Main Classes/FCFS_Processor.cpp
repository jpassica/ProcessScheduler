#include "FCFS_Processor.h"
#include "Scheduler.h"

FCFS_Processor::FCFS_Processor(int ID, Scheduler* pSch) : Processor(ID, pSch)
{
}

void FCFS_Processor::ScheduleAlgo()
{
}

int FCFS_Processor::CalcFinishTime()
{
	for (size_t i = 1; i <= FCFS_Ready.getLength(); i++)
	{
		finishTime += FCFS_Ready.getEntry(i)->GetCPUTime();
	}

	if (RunPtr)
		finishTime += RunPtr->GetCPUTime();

	return finishTime;
}

void FCFS_Processor::AddToReadyQueue(Process* pReady)
{
	FCFS_Ready.insert(FCFS_Ready.getCount() + 1, pReady);
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

	if (newRunPtr->isRecentlyUpdated(crntTimeStep))
		return false;

	RunPtr = newRunPtr;
	FCFS_Ready.remove(1);

	CrntState = BUSY;
	RunPtr->SetLastUpdateTime(crntTimeStep);
	RunPtr->ChangeProcessState(RUN);

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

		if (pScheduler->ToTRM(killedProcess))
		{
			FCFS_Ready.remove(position);
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
