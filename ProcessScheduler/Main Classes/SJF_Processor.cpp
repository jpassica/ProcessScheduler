#include "SJF_Processor.h"
#include "Scheduler.h"

SJF_Processor::SJF_Processor(int ID, Scheduler* SchedulerPtr) : Processor(ID, SchedulerPtr)
{}

void SJF_Processor::ScheduleAlgo(int CrntTimeStep)
{ 
	pScheduler->HandleIORequest(this);
	//Case 1: if there is no running process and the ready queue is empty, there is nothing to do for now

	//Case 2: if there is no running process but there is a process in the ready queue, move it to RUN
	if (!RunPtr && !SJF_Ready.isEmpty())
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

	//IO & others
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

bool SJF_Processor::RunNextProcess(int crntTimeStep)
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
	RunPtr->ChangeProcessState(RUN);

	if (RunPtr->isFirstExecution())
		RunPtr->SetResponseTime(crntTimeStep);

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
