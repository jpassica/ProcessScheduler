#include "RR_Processor.h"
#include "scheduler.h"
RR_Processor::RR_Processor(int ID, int timeSlice, Scheduler* SchedulerPtr) : Processor(ID, SchedulerPtr), TimeSlice(timeSlice)
{
	TimeSliceCounter = 0;
}

void RR_Processor::ScheduleAlgo(int CrntTimeStep)
{
	{//First, check if there is a IO Request to be handled at the current time step
		//if (RunPtr && RunPtr->TimeForIO())
		//{
		//	pScheduler->BlockProcess(RunPtr);
		//	RunPtr = nullptr;
		//	CrntState = IDLE;
		//}

		////if the process request IO or there is no running process -> pick the ready process to run
		//if (!RunPtr)
		//{
		////if the running process is done executing and is ready to move to TRM
		//if (RunPtr && RunPtr->GetProcessedTime() == RunPtr->GetCPUTime())
		//{
		//	pScheduler->TerminateProcess(RunPtr);
		//	RunPtr = nullptr;
		//	CrntState = IDLE;
		//	TimeSliceCounter = 0;
		//	if (RunNextProcess(CrntTimeStep))
		//		TimeSliceCounter++;

		//}

		////if there is no running process and the ready queue is empty,
		////then there is nothing to do
		//if (!RunPtr && RR_Ready.isEmpty()) {
		//	TimeSliceCounter = 0;
		//	RunNextProcess(CrntTimeStep);
		//}

		///*while (pScheduler->MigrateFromRRtoSJF(RunPtr)) 
		//{
		//	RunPtr = nullptr;
		//	TimeSliceCounter = 0;
		//	RunNextProcess(CrntTimeStep);
		//}*/

		////means that all processes migrate which means that ready queue is empty
		//if (!RunPtr)
		//{
		//	TimeSliceCounter = 0;
		//	CrntState = IDLE;
		//	return;
		//}


		////else if the running process is not done executing but has just finished it's time slice
		////then it goes back to RDY list 
		//else if (TimeSliceCounter == TimeSlice)
		//{
		//	AddToReadyQueue(RunPtr);
		//	RunPtr = nullptr;
		//	CrntState = IDLE;
		//	TimeSliceCounter = 0;
		//	RunNextProcess(CrntTimeStep);
		//}

		////else if there is a running process -> counter++
		//if (RunPtr)
		//{
		//	TimeSliceCounter++;
		//	RunPtr->ExecuteProcess();
		//	if (RunNextProcess(CrntTimeStep))
		//		TimeSliceCounter++;

		//}

		//IncrementBusyOrIdleTime();
		////else -> the process has not completed its time slice 
		//else
		//	TimeSliceCounter++;
		//return;
	}

	//IOOO
	
	//if there is no running process -> pick the ready process to run
	if (!RunPtr)
	{
		TimeSliceCounter = 0;
		RunNextProcess(CrntTimeStep);
	}

	//if the process request IO or there is no running process -> pick the ready process to run
	if (!RunPtr)
	{
		TimeSliceCounter = 0;
		if (!RunNextProcess(CrntTimeStep)) { // no runptr & no ready process
			CrntState = IDLE;
			return;
		}
	}

	//if the running process is done executing and is ready to move to TRM
	if (RunPtr->GetProcessedTime() == RunPtr->GetCPUTime()) 
	{
		pScheduler->TerminateProcess(RunPtr);
		RunPtr = nullptr;
		TimeSliceCounter = 0;
		RunNextProcess(CrntTimeStep);
	}

	while (pScheduler->MigrateFromRRtoSJF(RunPtr)) 
	{
		RunPtr = nullptr;
		TimeSliceCounter = 0;
		RunNextProcess(CrntTimeStep);
	}

	//means that all processes migrate which means that ready queue is empty
	if (!RunPtr)
	{
		TimeSliceCounter = 0;
		CrntState = IDLE;
		return;
	}

	//if the running process is not done executing but has just finished it's time slice
	//then it goes back to RDY list 
	if (TimeSliceCounter == TimeSlice) {
		AddToReadyQueue(RunPtr);
		RunPtr = nullptr;
		TimeSliceCounter = 0;
		RunNextProcess(CrntTimeStep);
	}

	//else if there is a running process -> counter++
	if (RunPtr)
		TimeSliceCounter++;
}
		
void RR_Processor::AddToReadyQueue(Process* pReady)
{
	RR_Ready.Enqueue(pReady);

	pReady->ChangeProcessState(RDY);

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

