#include "RR_Processor.h"
#include "scheduler.h"
RR_Processor::RR_Processor(int ID, int timeSlice, Scheduler* SchedulerPtr) : Processor(ID, SchedulerPtr), TimeSlice(timeSlice)
{
	TimeSliceCounter = 0;
}

void RR_Processor::ScheduleAlgo(int CrntTimeStep)
{
	pScheduler->HandleIORequest(this);
	//pScheduler->MigrateToFCFS(this); 

	//if there is no running process and the ready queue is empty,
	//then there is nothing to do
	if (!RunPtr && RR_Ready.isEmpty()) {
		TimeSliceCounter = 0;
		CrntState = IDLE;
		return;
	}

	//if there is no running process but there is a process in the ready queue, move it to RUN
	if (!RunPtr)
	{
		RunNextProcess(CrntTimeStep);
		TimeSliceCounter++;
		return;
	}

	//if the running process is done executing and is ready to move to TRM
	if (RunPtr->GetProcessedTime() == RunPtr->GetCPUTime())
	{
		pScheduler->TerminateProcess(RunPtr); 
		RunPtr = nullptr;
		CrntState = IDLE;
	    TimeSliceCounter = 0;
		if (RunNextProcess(CrntTimeStep))
			TimeSliceCounter++;
		
    }

	//else if the running process is not done executing but has just finished it's time slice
	//then it goes back to RDY list 
	else if (TimeSliceCounter == TimeSlice) 
	{
		AddToReadyQueue(RunPtr);
		RunPtr = nullptr;
		CrntState = IDLE;
		TimeSliceCounter = 0;
		if (RunNextProcess(CrntTimeStep)) 
			TimeSliceCounter++;

	}

	//else -> the process has not completed its time slice 
	else 
		TimeSliceCounter++;
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
	RunPtr->ChangeProcessState(RUN);

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
