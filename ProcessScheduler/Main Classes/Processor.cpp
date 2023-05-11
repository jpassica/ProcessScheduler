#include "Processor.h"

Processor::Processor(int ID, Scheduler* SchedulerPtr,int Healing) : CrntState(IDLE), ID(ID),HealingTime(Healing), RunPtr(nullptr), pScheduler(SchedulerPtr)
{
	//initializing all processor data members
	BusyTime = 0;
	IdleTime = 0;
	FinishTime = 0;
	HealingTimeCounter = HealingTime;
}

double Processor::CalcPLoad(int TotalTRT) const
{
	if (!TotalTRT)  return 0;

	return ((double)BusyTime / TotalTRT) * 100;
}

double Processor::CalcPUtil() const		
{
	return ((double)BusyTime / (BusyTime + IdleTime)) * 100;
}

int Processor::GetID() const
{
	return ID;
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

bool Processor::IsStopped() const
{
	return CrntState == STOP;
}

void Processor::ContinueHealing() {

	HealingTimeCounter--;
	if (HealingTimeCounter == 0)
	{
		CrntState = IDLE;
		HealingTimeCounter = HealingTime;
	}
}
