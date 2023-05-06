#include "FCFS_Processor.h"
#include "Scheduler.h"

FCFS_Processor::FCFS_Processor(int ID, int ForkProb, Scheduler* SchedulerPtr) : Processor(ID, SchedulerPtr), ForkProbability(ForkProb) {}

void FCFS_Processor::ScheduleAlgo(int CrntTimeStep)
{
	//Forking
	if (RunPtr)
	{
		int Random = rand() % 100;
		if (Random <= ForkProbability)
			pScheduler->Fork(RunPtr);
	}

	//KillBySig signals
	bool Kill = 1;
	while (!KillSignalQ.isEmpty() && KillSignalQ.QueueFront()->Time == CrntTimeStep && Kill)
		Kill = KillBySig();

	//IO requests
	if (RunPtr && RunPtr->TimeForIO())
	{
		pScheduler->BlockProcess(RunPtr);
		RunPtr = nullptr;
		CrntState = IDLE;							
	}

	//Case 1: if there is no running process and the ready list is empty, there is nothing to do for now

	//Case 2: if the running process is done executing and is ready to move to TRM
	if (RunPtr && RunPtr->GetRemainingCPUTime() <= 0)
	{
		pScheduler->TerminateProcess(RunPtr);
		RunPtr = nullptr;
		RunNextProcess(CrntTimeStep);
	}

	//Case 3: if there is no running process but there is a process in the ready list, move it to RUN

	else if (!RunPtr && !FCFS_Ready.isEmpty())
		RunNextProcess(CrntTimeStep);

	//Migration case	
	while (RunPtr && pScheduler->MigrateFromFCFStoRR(RunPtr))
	{
		RunPtr = nullptr;
		RunNextProcess(CrntTimeStep);
	}

	//Case4: if the running process is not done executing, then there is nothing to do for now

	if (RunPtr)
	{
		RunPtr->ExecuteProcess();
	}

	IncrementBusyOrIdleTime();
}

void FCFS_Processor::AddToReadyQueue(Process* pReady)
{
	FCFS_Ready.insert(FCFS_Ready.getCount() + 1, pReady);

	pReady->ChangeProcessState(RDY);

	FinishTime += pReady->GetRemainingCPUTime();
}

bool FCFS_Processor::isReadyQueueEmpty() const
{
	return FCFS_Ready.isEmpty();
}

bool FCFS_Processor::RunNextProcess(int crntTimeStep)
{
	if (RunPtr)
		return false;

	if (isReadyQueueEmpty())
	{
		CrntState = IDLE;
		return false;
	}

	Process* newRunPtr = FCFS_Ready.getEntry(1);

	RunPtr = newRunPtr;

	FCFS_Ready.remove(1);

	CrntState = BUSY;
	RunPtr->ChangeProcessState(RUN);

	if (RunPtr->isFirstExecution())
		RunPtr->SetResponseTime(crntTimeStep);

	FinishTime -= RunPtr->GetRemainingCPUTime();

	return true;
}

int FCFS_Processor::GetRDYCount() const
{
	return FCFS_Ready.getCount();
}

bool FCFS_Processor::KillByID(int ID)
{
	int position = FCFS_Ready.SearchByID(ID);

	if (position)
	{
		Process* killedProcess = FCFS_Ready.getEntry(position);

		pScheduler->TerminateProcess(killedProcess);

		FCFS_Ready.remove(position);
			
		FinishTime -= killedProcess->GetRemainingCPUTime();

		return true;
	}
	else
		return false;
}

void FCFS_Processor::PrintRDY() const
{
	FCFS_Ready.Print();
}

Process* FCFS_Processor::StealProcess()
{
	Process* StolenProcess = FCFS_Ready.getEntry(1);

	FCFS_Ready.remove(1);

	FinishTime -= StolenProcess->GetRemainingCPUTime();

	return StolenProcess;
}

bool FCFS_Processor::KillBySig()
{
	KillSignal* KillSig = KillSignalQ.QueueFront();

	// if the process to be killed is the runptr 
	if (RunPtr && RunPtr->GetID() == KillSig->PID)
	{
		KillSignalQ.Dequeue(KillSig);

		pScheduler->TerminateProcess(RunPtr);          // terminate the process

		RunPtr = nullptr;

		pScheduler->IncrementKillCount();

		delete KillSig;

		return true;
	}

	// If the process to be killed is found in the ready list of an FCFS processor
	else if (KillByID(KillSig->PID))
	{
		KillSignalQ.Dequeue(KillSig);

		pScheduler->IncrementKillCount();

		delete KillSig;

		return true;
	}

	// Not RDY/RUN for FCFS -> ignore
	return false;
}

bool FCFS_Processor::KillOrphan(int ID)
{
	//If the orphan is the running process
	if (RunPtr && RunPtr->GetID() == ID)
	{
		pScheduler->TerminateProcess(RunPtr);
		RunPtr = nullptr;
		CrntState = IDLE;

		return true;
	}
	else if (KillByID(ID))			//If the orphan is in the ready list
		return true;
	
	//If orphan is not found anywhere
	return false;
}

Queue<KillSignal*> FCFS_Processor::KillSignalQ;