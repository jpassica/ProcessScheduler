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

bool FCFS_Processor::fromReadyToRun( int crntTimeStep)
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

void FCFS_Processor::RandomKill(int randomID)
{
	bool found = 0;
	Process* killPtr(nullptr);

	for (int j = 1; j <= FCFS_Ready.getCount() && !found; j++)
	{
		killPtr = FCFS_Ready.getEntry(j);

		if (killPtr->GetPID() == randomID && pScheduler->ToTRM(killPtr))		//checking process matching & if able to terminate
		{
				FCFS_Ready.remove(j);									//removing from RDY_List
				found = true;
		}
	}
}

void FCFS_Processor::printRDY() const
{
	FCFS_Ready.Print();
}
