#include "Processor.h"

Processor::Processor(int ID) : CrntState(IDLE), ID(ID), RunPtr(nullptr)
{
	//initializing all processor data members
	busyTime = 0;
	idleTime = 0;
	finishTime = 0;
}

double Processor::CalcPLoad(int TotalTRT)		//calculates and returns pLoad %
{
	return ((double)busyTime / TotalTRT) * 100;
}

double Processor::CalcPUtil() const				//calculates and returns pUtil %
{
	return ((double)busyTime / (busyTime + idleTime)) * 100;
}

int Processor::getID()
{
	return ID;
}

ostream& operator<<(ostream& out, const Processor& P)
{
	out << P.ID;
	return out;
}
