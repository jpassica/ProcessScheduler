#include "FCFS_Processor.h"
#include "Scheduler.h"

FCFS_Processor::FCFS_Processor(int ID, int ForkProb, Scheduler* SchedulerPtr, int HealingTime) : Processor(ID, SchedulerPtr, HealingTime), ForkProbability(ForkProb) {}

void FCFS_Processor::ScheduleAlgo(int CrntTimeStep)
{
	//If the processor is in STOP state
	if (CrntState == STOP) 
	{
		ContinueHealing();
		return;
	}

	//Executing kill signals
	bool Kill = 1;
	while (!KillSignalQ.isEmpty() && KillSignalQ.QueueFront()->Time == CrntTimeStep && Kill)
		Kill = ExecuteKillSignal();

	//Handling IO requests
	if (RunPtr && RunPtr->TimeForIO())
	{
		pScheduler->BlockProcess(RunPtr);
		RunPtr = nullptr;
		CrntState = IDLE;							
	}

	//Forking
	if (RunPtr && RunPtr->CanFork())
	{
		if (rand() % 1000 < ForkProbability)
		{
			pScheduler->Fork(RunPtr);
		}
	}

	//If the running process is done executing and is ready to move to TRM
	if (RunPtr && RunPtr->GetRemainingCPUTime() <= 0)
	{
		pScheduler->TerminateProcess(RunPtr);
		RunPtr = nullptr;
		RunNextProcess(CrntTimeStep);
	}

	//If there is no running process but there is a process in the ready list, move it to RUN
	else if (!RunPtr && !FCFS_Ready.isEmpty())
		RunNextProcess(CrntTimeStep);

	//Migration case	
	while (RunPtr && !RunPtr->IsChild() && pScheduler->MigrateFromFCFStoRR(RunPtr))
	{
		RunPtr = nullptr;
		RunNextProcess(CrntTimeStep);
	}

	if (RunPtr)
		RunPtr->ExecuteProcess();

	IncrementBusyOrIdleTime();
}

void FCFS_Processor::AddToReadyQueue(Process* pReady)
{
	FCFS_Ready.insert(FCFS_Ready.getCount() + 1, pReady);

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
	size_t i(1);

	//Skipping any forked processes, but not changing their position in the list
	while (i <= FCFS_Ready.getCount() && FCFS_Ready.getEntry(i)->IsChild())
	{
		i++;
	}

	//If there are no non-forked processes in the ready list, then we can't steal anything
	if (i > FCFS_Ready.getCount())
		return nullptr;

	Process* StolenProcess = FCFS_Ready.getEntry(i);

	FCFS_Ready.remove(i);

	FinishTime -= StolenProcess->GetRemainingCPUTime();

	return StolenProcess;
}

void FCFS_Processor::ClearOverheatedProcessor() 
{
	if (RunPtr) 
		pScheduler->MovetoRDY(RunPtr);


	while (!FCFS_Ready.isEmpty())
	{
		pScheduler->MovetoRDY(FCFS_Ready.getEntry(1));
		FCFS_Ready.remove(1);
	}
	
	RunPtr = nullptr;
	CrntState = STOP;
	FinishTime = 0;
}

bool FCFS_Processor::ExecuteKillSignal()
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

void FCFS_Processor::AddKillSignal(int Time, int ID)
{
	KillSignal* NewKillSignal = new KillSignal(Time, ID);

	KillSignalQ.Enqueue(NewKillSignal);
}

Queue<KillSignal*> FCFS_Processor::KillSignalQ;

void FCFS_Processor::ClearKillSignalQ()
{
	KillSignal* DeleteKillSig = nullptr;

	while (!KillSignalQ.isEmpty())
	{
		KillSignalQ.Dequeue(DeleteKillSig);
		delete DeleteKillSig;
	}
}

void FCFS_Processor::RemoveIgnoredKillSignals(int CrntTimeStep)
{
	KillSignal* IgnoredKillSig = nullptr;

	while (KillSignalQ.QueueFront()->Time == CrntTimeStep)
	{
		KillSignalQ.Dequeue(IgnoredKillSig);
		delete IgnoredKillSig;
	}
}
