#include "Processor.h"

Processor::Processor(int ID, Scheduler* SchedulerPtr) : CrntState(IDLE), ID(ID), RunPtr(nullptr), pScheduler(SchedulerPtr)
{
	//initializing all processor data members
	BusyTime = 0;
	IdleTime = 0;
	FinishTime = 0;
}

double Processor::CalcPLoad(int TotalTRT) const
{
	return ((double)BusyTime / TotalTRT) * 100;
}

double Processor::CalcPUtil() const		
{
	return ((double)BusyTime / (BusyTime + IdleTime)) * 100;
}

void Processor::ChangeProcessorState(ProcessorState NextState)
{
	CrntState = NextState;
}

int Processor::GetID() const
{
	return ID;
}

ProcessorState Processor::GetProcessorState()
{
	return CrntState;
}

void Processor::IncrementBusyOrIdleTime()
{
	if (CrntState == IDLE)
		IdleTime++;
	else
		BusyTime++;
}

bool Processor::isExecutingProcess() const
{
	return RunPtr;
}

int Processor::GetRunningProcessID() const
{
	if (RunPtr)
		return RunPtr->GetID();
	else
		return 0;
}

ostream& operator<<(ostream& out, const Processor& P)
{
	out << P.ID;
	return out;
}

int Processor::GetFinishTime() const
{
	return FinishTime;
}

Queue<KillSignal*> Processor::KillSignalQ;