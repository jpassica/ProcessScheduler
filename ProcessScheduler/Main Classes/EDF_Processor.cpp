#include "EDF_Processor.h"
#include "Scheduler.h"

EDF_Processor::EDF_Processor(int ID, Scheduler* SchedulerPtr, int HealingTime) : Processor(ID, SchedulerPtr, HealingTime) {}

void EDF_Processor::ScheduleAlgo(int CrntTimeStep)
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

	//if there is no running process but there is a process in the ready queue, move it to RUN
	if (!RunPtr && !EDF_Ready.isEmpty())
		RunNextProcess(CrntTimeStep);

	//If the running process is done executing and is ready to move to TRM
	else if (RunPtr && RunPtr->GetRemainingCPUTime() <= 0)
	{
		pScheduler->TerminateProcess(RunPtr);
		RunPtr = nullptr;
		RunNextProcess(CrntTimeStep);
	}

	//If there is a running process but there arrives a new one with a sooner deadline,
	//move that one to run instead

	else if (!EDF_Ready.isEmpty() && RunPtr)
	{
		Process* SoonerDeadline = nullptr;
		EDF_Ready.QueueFront(SoonerDeadline);

		if (SoonerDeadline->GetDeadline() < RunPtr->GetDeadline())
		{
			//Return the process to the ready queue
			AddToReadyQueue(RunPtr);

			RunPtr = nullptr;

			//Run the process with the shortest deadline
			RunNextProcess(CrntTimeStep);
		}
	}

	if (RunPtr)
		RunPtr->ExecuteProcess();

	IncrementBusyOrIdleTime();
}

void EDF_Processor::AddToReadyQueue(Process* pReady)
{
	EDF_Ready.Enqueue(pReady, pReady->GetDeadline());

	FinishTime += pReady->GetRemainingCPUTime();
}

bool EDF_Processor::isReadyQueueEmpty() const
{
	return EDF_Ready.isEmpty();
}

void EDF_Processor::ClearOverheatedProcessor() {

	Process* ProcessToMove;
	if (RunPtr) {
		pScheduler->MovetoRDY(RunPtr);
	}

	while (!EDF_Ready.isEmpty())
	{
		EDF_Ready.Dequeue(ProcessToMove);
		pScheduler->MovetoRDY(ProcessToMove);
	}

	RunPtr = nullptr;
	CrntState = STOP;
	FinishTime = 0;
}

bool EDF_Processor::RunNextProcess(int crntTimeStep)
{
	if (RunPtr)
		return false;

	if (isReadyQueueEmpty())
	{
		CrntState = IDLE;
		return false;
	}

	EDF_Ready.Dequeue(RunPtr);

	CrntState = BUSY;

	if (RunPtr->isFirstExecution())
		RunPtr->SetResponseTime(crntTimeStep);

	FinishTime -= RunPtr->GetRemainingCPUTime();

	return true;
}

int EDF_Processor::GetRDYCount() const
{
	return EDF_Ready.getCount();
}

void EDF_Processor::PrintRDY() const
{
	EDF_Ready.Print();
}

Process* EDF_Processor::StealProcess()
{
	Process* StolenProcess = nullptr;

	EDF_Ready.Dequeue(StolenProcess);

	FinishTime -= StolenProcess->GetRemainingCPUTime();

	return StolenProcess;
}
