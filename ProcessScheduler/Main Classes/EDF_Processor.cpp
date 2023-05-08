#include "EDF_Processor.h"
#include "Scheduler.h"

EDF_Processor::EDF_Processor(int ID, Scheduler* SchedulerPtr) : Processor(ID, SchedulerPtr) {}

void EDF_Processor::ScheduleAlgo(int CrntTimeStep)
{
	//First, check if there is an IO Request to be handled at the current Time step
	if (RunPtr && RunPtr->TimeForIO())
	{
		pScheduler->BlockProcess(RunPtr);
		RunPtr = nullptr;
		CrntState = IDLE;
	}

	//If there is no running process and the ready queue is empty, there is nothing to do for now

	//if there is no running process but there is a process in the ready queue, move it to RUN
	if (!RunPtr && !EDF_Ready.isEmpty())
		RunNextProcess(CrntTimeStep);

	//if the running process is done executing and is ready to move to TRM
	else if (RunPtr && !RunPtr->GetRemainingCPUTime())
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

	//if the running process is not done executing, then there is nothing to do for now

	if (RunPtr)
	{
		RunPtr->ExecuteProcess();
	}

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
