#include "RR_Processor.h"
#include "scheduler.h"

RR_Processor::RR_Processor(int ID, int timeSlice, Scheduler* SchedulerPtr) : Processor(ID, SchedulerPtr), TimeSlice(timeSlice)
{
	TimeSliceCounter = 0;
}

void RR_Processor::ScheduleAlgo(int CrntTimeStep)
{
	//First, check if there is an IO Request to be handled at the current Time step
	if (RunPtr && RunPtr->TimeForIO())
	{
		pScheduler->BlockProcess(RunPtr);
		RunPtr = nullptr;
		CrntState = IDLE;
	}
	
	//if there is no running process -> pick the next process to run
	if (!RunPtr)
	{
		TimeSliceCounter = 0;
		RunNextProcess(CrntTimeStep);
	}

	//if the running process is done executing and is ready to move to TRM
	else if (RunPtr && RunPtr->GetProcessedTime() == RunPtr->GetCPUTime()) 
	{
		pScheduler->TerminateProcess(RunPtr);
		RunPtr = nullptr;
		TimeSliceCounter = 0;
		RunNextProcess(CrntTimeStep);
	}

	//Migration
	while (RunPtr && pScheduler->MigrateFromRRtoSJF(RunPtr)) 
	{
		RunPtr = nullptr;
		TimeSliceCounter = 0;
		RunNextProcess(CrntTimeStep);
	}

	//If the ready queue has become empty after migration
	if (!RunPtr)
	{
		TimeSliceCounter = 0;
		CrntState = IDLE;
	}

	//If the running process is not done executing but has just finished its Time slice
	//then it goes back to RDY list 
	else if (TimeSliceCounter == TimeSlice) 
	{
		AddToReadyQueue(RunPtr);
		RunPtr = nullptr;
		TimeSliceCounter = 0;
		RunNextProcess(CrntTimeStep);
	}

	//else if there is a running process -> counter++
	if (RunPtr)
	{
		TimeSliceCounter++;
		RunPtr->ExecuteProcess();
	}
	IncrementBusyOrIdleTime();
}
		
void RR_Processor::AddToReadyQueue(Process* pReady)
{
	RR_Ready.Enqueue(pReady);

	FinishTime += pReady->GetRemainingCPUTime();
}

bool RR_Processor::isReadyQueueEmpty() const
{
	return RR_Ready.isEmpty();
}

bool RR_Processor::RunNextProcess(int crntTimeStep)
{
	if (RunPtr)
		return false;

	if (isReadyQueueEmpty())
	{
		CrntState = IDLE;
		return false;
	}

	RR_Ready.Dequeue(RunPtr);

	CrntState = BUSY;

	if (RunPtr->isFirstExecution())
		RunPtr->SetResponseTime(crntTimeStep);

	FinishTime -= RunPtr->GetRemainingCPUTime();

	return true;
}

int RR_Processor::GetRDYCount() const
{
	return RR_Ready.getCount();
}

void RR_Processor::PrintRDY() const
{
	RR_Ready.Print();
}

Process* RR_Processor::StealProcess()
{
	Process* StolenProcess = nullptr;

	RR_Ready.Dequeue(StolenProcess);

	FinishTime -= StolenProcess->GetRemainingCPUTime();

	return StolenProcess;
}

