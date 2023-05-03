#include "EDF_Processor.h"
#include "Scheduler.h"

EDF_Processor::EDF_Processor(int ID, Scheduler* SchedulerPtr) : Processor(ID, SchedulerPtr)
{
}

void EDF_Processor::ScheduleAlgo(int CrntTimeStep)
{
	pScheduler->HandleIORequest(this);

	//If there is no running process and the ready list is empty, there is nothing to do for now

	//if there is no running process but there is a process in the ready queue, move it to RUN
	if (!RunPtr && !EDF_Ready.isEmpty())
	{
		RunNextProcess(CrntTimeStep);
		return;
	}

	//If there is a running process but there arrives a new one with a sooner deadline,
	//move that one to run instead

	/*if (!EDF_Ready.isEmpty() && RunPtr)
	{
		Process* SoonerDeadline = nullptr;
		EDF_Ready.QueueFront(SoonerDeadline);

		if (SoonerDeadline->GetDeadline() < RunPtr->GetDeadline())
		{
			RunPtr->ChangeProcessState(RDY);
			EDF_Ready.Enqueue(RunPtr, RunPtr->GetDeadline());

			RunPtr->ChangeProcessState(RUN);
			RunPtr = SoonerDeadline;
		}
	}*/

	//if the running process is done executing and is ready to move to TRM
	if (RunPtr && !RunPtr->GetRemainingCPUTime())
	{
		pScheduler->TerminateProcess(RunPtr);
		RunPtr = nullptr;
		CrntState = IDLE;
		RunNextProcess(CrntTimeStep);
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
	return EDF_Ready.isEmpty();
}

bool EDF_Processor::RunNextProcess(int crntTimeStep)
{
	if (RunPtr || CrntState == BUSY)
		return false;

	if (isReadyQueueEmpty())
		return false;

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
