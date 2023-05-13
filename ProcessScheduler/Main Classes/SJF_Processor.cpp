#include "SJF_Processor.h"
#include "Scheduler.h"

SJF_Processor::SJF_Processor(int ID, Scheduler* SchedulerPtr, int HealingTime) : Processor(ID, SchedulerPtr, HealingTime)
{}

void SJF_Processor::ScheduleAlgo(int CrntTimeStep)
{ 
	//If the processor is in STOP state
	if (CrntState == STOP)
	{
		ContinueHealing();
		return;
	}

	//Handling IO requests
	if (RunPtr && RunPtr->TimeForIO())
	{
		pScheduler->BlockProcess(RunPtr);
		RunPtr = nullptr;
		CrntState = IDLE;
	}

	//If the running process is done executing and is ready to move to TRM
	if (RunPtr && RunPtr->GetRemainingCPUTime() <= 0)
	{
		pScheduler->TerminateProcess(RunPtr);
		RunPtr = nullptr;
		RunNextProcess(CrntTimeStep);
	}

	//If there is no running process but there is a process in the ready queue, move it to RUN
	else if (!RunPtr && !SJF_Ready.isEmpty())
	{
		RunNextProcess(CrntTimeStep);
	}

	if (RunPtr)
		RunPtr->ExecuteProcess();

	IncrementBusyOrIdleTime();
}

void SJF_Processor::AddToReadyQueue(Process* pReady)
{
	SJF_Ready.Enqueue(pReady, pReady->GetCPUTime());

	FinishTime += pReady->GetRemainingCPUTime();
}

bool SJF_Processor::isReadyQueueEmpty() const
{
	return SJF_Ready.isEmpty();
}

void SJF_Processor::ClearOverheatedProcessor() {

	Process* ProcessToMove;
	if (RunPtr) {
		pScheduler->MovetoRDY(RunPtr);
	}

	while (!SJF_Ready.isEmpty())
	{
		SJF_Ready.Dequeue(ProcessToMove);
		pScheduler->MovetoRDY(ProcessToMove);
	}

	RunPtr = nullptr;
	CrntState = STOP;
	FinishTime = 0;
}

bool SJF_Processor::RunNextProcess(int CrntTimeStep)
{
	if (RunPtr)
		return false;

	if (isReadyQueueEmpty())
	{
		CrntState = IDLE;
		return false;
	}

	SJF_Ready.Dequeue(RunPtr);

	CrntState = BUSY;

	if (RunPtr->isFirstExecution())
		RunPtr->SetResponseTime(CrntTimeStep);

	FinishTime -= RunPtr->GetRemainingCPUTime();

	return true;
}

int SJF_Processor::GetRDYCount() const
{
	return SJF_Ready.getCount();
}

void SJF_Processor::PrintRDY() const
{
	SJF_Ready.Print();
}

Process* SJF_Processor::StealProcess()
{
	Process* StolenProcess = nullptr;

	SJF_Ready.Dequeue(StolenProcess);

	FinishTime -= StolenProcess->GetRemainingCPUTime();

	return StolenProcess;
}
