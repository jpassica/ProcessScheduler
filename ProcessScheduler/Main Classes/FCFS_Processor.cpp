#include "FCFS_Processor.h"

FCFS_Processor::FCFS_Processor(int ID , scheduler* pSch) : Processor(ID , pSch)
{
}

void FCFS_Processor::ScheduleAlgo()
{
	//assuming this function is only called when necessary, 
	//when the running process is done executing

	//1: move done process to TRM list
	//move

	//2: place next process (if exists) in RUN state
	if (!FCFS_Ready.isEmpty())						//if there is a process to run next
	{
		RunPtr = FCFS_Ready.getEntry(1);
		FCFS_Ready.remove(1);
		RunPtr->ChangeProcessState(RUN);
	}
	else										//there is no process to run
	{
		RunPtr = nullptr;
	}


	if (RunPtr)				//if there is a process currently running
		CrntState = BUSY;
	else
		CrntState = IDLE;
}

int FCFS_Processor::CalcFinishTime()
{
	for (size_t i = 1; i <= FCFS_Ready.getLength(); i++)
	{
		finishTime += FCFS_Ready.getEntry(i)->GetCPUTime();
	}
	
	if (RunPtr)
		finishTime += RunPtr->GetCPUTime();

	return finishTime;
}

List<Process*>& FCFS_Processor::getRDY()
{
	return FCFS_Ready;
}
