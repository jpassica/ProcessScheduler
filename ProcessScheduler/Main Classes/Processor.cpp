#include "Processor.h"

Processor::Processor(int ID, Scheduler* sch) : CrntState(IDLE), ID(ID), RunPtr(nullptr), pScheduler(sch)
{
	//initializing all processor data members
	busyTime = 0;
	idleTime = 0;
	finishTime = 0;
}

double Processor::CalcPLoad(int TotalTRT) const	//calculates and returns pLoad %
{
	return ((double)busyTime / TotalTRT) * 100;
}

double Processor::CalcPUtil() const				//calculates and returns pUtil %
{
	return ((double)busyTime / (busyTime + idleTime)) * 100;
}

bool Processor::DecrementRunningProcess()
{
	if (!RunPtr)
		return false;
	
	RunPtr->RunProcess();
	return true;
}

void Processor::IncrementBusyOrIdleTime()
{
	if (CrntState == IDLE)
		idleTime++;
	else
		busyTime++;
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

ProcessorState Processor::getProcessorState()
{
	return CrntState;
}

Process* Processor::getRunPtr()
{
	return RunPtr;
}

ostream& operator<<(ostream& out, const Processor& P)
{
	out << P.ID;
	return out;
}
