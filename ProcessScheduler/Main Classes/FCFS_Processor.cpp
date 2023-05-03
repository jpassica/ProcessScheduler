#include "FCFS_Processor.h"
#include "Scheduler.h"

FCFS_Processor::FCFS_Processor(int ID, Scheduler* SchedulerPtr) : Processor(ID, SchedulerPtr) {}

void FCFS_Processor::ScheduleAlgo(int CrntTimeStep)
{
	pScheduler->HandleIORequest(this);

	//Case 1: if there is no running process and the ready list is empty, there is nothing to do for now

	//Case 2: if there is no running process but there is a process in the ready list, move it to RUN
	if (!RunPtr && !FCFS_Ready.isEmpty())
	{
		RunNextProcess(CrntTimeStep);
	}

	//Case 3: if the running process is done executing and is ready to move to TRM
	else if (RunPtr && !RunPtr->GetRemainingCPUTime())
	{
		pScheduler->TerminateProcess(RunPtr);
		RunPtr = nullptr;
		CrntState = IDLE;
		RunNextProcess(CrntTimeStep);
	}

	//Case4: if the running process is not done executing, then there is nothing to do for now

	//IO, killsigs & others
}

void FCFS_Processor::AddToReadyQueue(Process* pReady)
{
	FCFS_Ready.insert(FCFS_Ready.getCount() + 1, pReady);

	FinishTime += pReady->GetRemainingCPUTime();
}

bool FCFS_Processor::isReadyQueueEmpty() const
{
	return FCFS_Ready.isEmpty();
}

bool FCFS_Processor::RunNextProcess(int crntTimeStep)
{
	if (RunPtr)
		return false;

	if (isReadyQueueEmpty())
	{
		CrntState = IDLE;
		return false;
	}

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

bool FCFS_Processor::KillByID(int ID)
{
	int position = FCFS_Ready.SearchByID(ID);

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

void FCFS_Processor::PrintRDY() const
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
