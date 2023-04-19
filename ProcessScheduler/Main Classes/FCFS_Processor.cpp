#include "FCFS_Processor.h"
#include "Scheduler.h"

FCFS_Processor::FCFS_Processor(int ID , Scheduler* pSch) : Processor(ID , pSch)
{
}

void FCFS_Processor::ScheduleAlgo()
{
}

int FCFS_Processor::CalcFinishTime()
{
	for (size_t i = 1; i <= ReadyList.getLength(); i++)
	{
		finishTime += ReadyList.getEntry(i)->GetCPUTime();
	}
	
	if (RunPtr)
		finishTime += RunPtr->GetCPUTime();

	return finishTime;
}

void FCFS_Processor::AddToReadyQueue(Process* pReady)
{
	ReadyList.insert(ReadyList.getCount() + 1, pReady);
}

bool FCFS_Processor::isReadyQueueEmpty() const
{
	if (ReadyList.isEmpty())
		return true;
	else
		return false;
}

bool FCFS_Processor::fromReadyToRun( int crntTimeStep)
{
	if (RunPtr || CrntState == BUSY)
		return false;

	if (isReadyQueueEmpty())
		return false;

	Process* newRunPtr = ReadyList.getEntry(1);

	if (newRunPtr->isRecentlyUpdated(crntTimeStep))
		return false;
	
	RunPtr = newRunPtr;
	ReadyList.remove(1);

	CrntState = BUSY;
	RunPtr->SetLastUpdateTime(crntTimeStep);
	RunPtr->ChangeProcessState(RUN);

	return true;
}

int FCFS_Processor::GetRDYCount() const
{
	return ReadyList.getCount();
}

void FCFS_Processor::RandomKill(int randomID)
{
	bool found = 0;
	Process* killPtr(nullptr);

	for (int j = 1; j <= ReadyList.getCount() && !found; j++)
	{
		killPtr = ReadyList.getEntry(j);

		if (killPtr->GetPID() == randomID && pScheduler->ToTRM(killPtr))		//checking process matching & if able to terminate
		{
				ReadyList.remove(j);									//removing from RDY_List
				found = true;
		}
	}
}

void FCFS_Processor::printRDY() const
{
	ReadyList.Print();
}
