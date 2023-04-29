#include "SJF_Processor.h"
#include "scheduler.h"

SJF_Processor::SJF_Processor(int ID, Scheduler* SchedulerPtr) : Processor(ID, SchedulerPtr)
{}

void SJF_Processor::ScheduleAlgo(int CrntTimeStep)
{ 
	//if there is no running process and the ready queue is empty,
	//then there is nothing to do
	if (!RunPtr && SJF_Ready.isEmpty())
		return;

	//if there is no running process but there is a process in the ready queue, move it to RUN
	if (!RunPtr)
	{
		SJF_Ready.Dequeue(RunPtr);
		RunPtr->ChangeProcessState(RUN);

		if (RunPtr->isFirstExecution())
			RunPtr->SetResponseTime(CrntTimeStep);
		return;
	}

	//if the running process is done executing and is ready to move to TRM
	if (RunPtr && !RunPtr->GetRemainingCPUTime())
	{
		pScheduler->ToTRM(RunPtr);

		//adding the next process to run
		if (!SJF_Ready.isEmpty())
		{
			SJF_Ready.Dequeue(RunPtr);
			RunPtr->ChangeProcessState(RUN);

			if (RunPtr->isFirstExecution())
				RunPtr->SetResponseTime(CrntTimeStep);
		}
		else
			RunPtr = nullptr;
	}
		//if the running process is not done executing, then there is nothing to do for now
	

}

void SJF_Processor::AddToReadyQueue(Process* pReady)
{
	SJF_Ready.Enqueue(pReady, pReady->GetCPUTime());

	FinishTime += pReady->GetCPUTime();
}

bool SJF_Processor::isReadyQueueEmpty() const
{
	if (SJF_Ready.isEmpty())
		return true;
	else
		return false;
}

bool SJF_Processor::fromReadyToRun(int crntTimeStep)
{
	if (RunPtr || CrntState == BUSY)
		return false;

	if (isReadyQueueEmpty())
		return false;

	Process* newRunPtr(nullptr);

	SJF_Ready.QueueFront(newRunPtr);

	SJF_Ready.Dequeue(RunPtr);

	CrntState = BUSY;
	RunPtr->ChangeProcessState(RUN);

	if (RunPtr->isFirstExecution())
		RunPtr->SetResponseTime(crntTimeStep);

	return true;
}

int SJF_Processor::GetRDYCount() const
{
	return SJF_Ready.getCount();
}

int SJF_Processor::GetFinishTime() const
{
	return FinishTime;
}

void SJF_Processor::printRDY() const
{
	SJF_Ready.Print();
}

Process* SJF_Processor::StealProcess()
{
	Process* StolenProcess = nullptr;

	SJF_Ready.Dequeue(StolenProcess);

	FinishTime -= StolenProcess->GetCPUTime();

	return StolenProcess;
}
