#include "FCFS_Processor.h"

FCFS_Processor::FCFS_Processor(int ID , scheduler* pSch) : Processor(ID , pSch)
{
}

void FCFS_Processor::ScheduleAlgo()
{
	//assuming this function is only called when necessary, 
	//when the running process is done executing

	//1: move done process to TRM list
	//move

	//2: place next process (if exists) in RUN state
	if (!ReadyList.isEmpty())						//if there is a process to run next
	{
		RunPtr = ReadyList.getEntry(1);
		ReadyList.remove(1);
		RunPtr->ChangeProcessState(RUN);
	}
	else										//there is no process to run
	{
		RunPtr = nullptr;
	}


	if (RunPtr)				//if there is a process currently running
		CrntState = BUSY;
	else
		CrntState = IDLE;
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

List<Process*>& FCFS_Processor::getRDY()
{
	return ReadyList;
}
