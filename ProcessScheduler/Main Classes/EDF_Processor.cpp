#include "EDF_Processor.h"
#include "Scheduler.h"

EDF_Processor::EDF_Processor(int ID, Scheduler* SchedulerPtr) : Processor(ID, SchedulerPtr)
{
}

void EDF_Processor::ScheduleAlgo(int CrntTimeStep)
{
	//If there is no running process and the ready list is empty, there is nothing to do for now
	if (!RunPtr && EDF_Ready.isEmpty())
	{
		CrntState = IDLE;
		return;
	}

	//if there is no running process but there is a process in the ready queue, move it to RUN
	if (!RunPtr)
	{
		EDF_Ready.Dequeue(RunPtr);
		RunPtr->ChangeProcessState(RUN);

		FinishTime -= RunPtr->GetRemainingCPUTime();

		if (RunPtr->isFirstExecution())
			RunPtr->SetResponseTime(CrntTimeStep);

		CrntState = BUSY;

		return;
	}

	//If there is a running process but there arrives a new one with a sooner deadline,
	//move that one to run instead

	Process* SoonerDeadline = nullptr;
	EDF_Ready.QueueFront(SoonerDeadline);

	if (RunPtr && SoonerDeadline && SoonerDeadline->GetDeadline() < RunPtr->GetDeadline())
	{
		RunPtr->ChangeProcessState(RDY);
		EDF_Ready.Enqueue(RunPtr, RunPtr->GetDeadline());

		RunPtr = SoonerDeadline;
		RunPtr->ChangeProcessState(RUN);
	}

	//if the running process is done executing and is ready to move to TRM
	if (RunPtr && !RunPtr->GetRemainingCPUTime())
	{
		pScheduler->TerminateProcess(RunPtr);

		//adding the next process to run
		if (!EDF_Ready.isEmpty())
		{
			EDF_Ready.Dequeue(RunPtr);
			RunPtr->ChangeProcessState(RUN);

			if (RunPtr->isFirstExecution())
				RunPtr->SetResponseTime(CrntTimeStep);
		}
		else
			RunPtr = nullptr;
	}
	//if the running process is not done executing, then there is nothing to do for now
}

void EDF_Processor::AddToReadyQueue(Process* pReady)
{
	EDF_Ready.Enqueue(pReady, pReady->GetCPUTime());

	FinishTime += pReady->GetRemainingCPUTime();
}

bool EDF_Processor::isReadyQueueEmpty() const
{
	if (EDF_Ready.isEmpty())
		return true;
	else
		return false;
}

bool EDF_Processor::fromReadyToRun(int crntTimeStep)
{
	if (RunPtr || CrntState == BUSY)
		return false;

	if (isReadyQueueEmpty())
		return false;

	Process* newRunPtr(nullptr);

	EDF_Ready.QueueFront(newRunPtr);

	EDF_Ready.Dequeue(RunPtr);

	CrntState = BUSY;
	RunPtr->ChangeProcessState(RUN);

	if (RunPtr->isFirstExecution())
		RunPtr->SetResponseTime(crntTimeStep);

	FinishTime -= RunPtr->GetRemainingCPUTime();

	return true;
}

int EDF_Processor::GetRDYCount() const
{
	return EDF_Ready.getCount();
}

void EDF_Processor::printRDY() const
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
