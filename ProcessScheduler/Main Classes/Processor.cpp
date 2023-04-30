#include "Processor.h"

Processor::Processor(int ID, Scheduler* SchedulerPtr) : CrntState(IDLE), ID(ID), RunPtr(nullptr), pScheduler(SchedulerPtr)
{
	//initializing all processor data members
	BusyTime = 0;
	IdleTime = 0;
	FinishTime = 0;
}

double Processor::CalcPLoad(int TotalTRT) const	//calculates and returns pLoad %
{
	return ((double)BusyTime / TotalTRT) * 100;
}

double Processor::CalcPUtil() const				//calculates and returns pUtil %
{
	return ((double)BusyTime / (BusyTime + IdleTime)) * 100;
}

void Processor::IncrementRunningProcess()
{
	if (RunPtr)
		RunPtr->ExecuteProcess();
}

void Processor::IncrementBusyOrIdleTime()
{
	if (CrntState == IDLE)
		IdleTime++;
	else
		BusyTime++;
}

void Processor::FlipProcessorState()
{
	if (CrntState == IDLE)
		CrntState = BUSY;
	else
		CrntState = IDLE;
}

int Processor::getID() const
{
	return ID;
}

void Processor::setRunptr(Process* p)
{
	RunPtr = p;
}

ProcessorState Processor::GetProcessorState()
{
	return CrntState;
}

Process* Processor::GetRunPtr()
{
	return RunPtr;
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
