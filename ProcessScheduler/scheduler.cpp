#include "scheduler.h"

scheduler::scheduler()
{
	Timestep = 0;

	FCFS_List = nullptr;
	SJF_List=nullptr;
	RR_List=nullptr;

	FCFSCount=0;
	SJFCount=0;
	RRCount=0;
	ProcessorsCount=0;

	RUN = nullptr;

	RTFCount = 0;
	MaxWCount = 0;
	STLCount = 0;
	Forkcount = 0;
	KillCount = 0;
}

int scheduler::getTimeStep()
{
	return Timestep;
}

Processor* scheduler::getFCFS_List()
{
	return FCFS_List;
}

Processor* scheduler::getSJF_List()
{
	return SJF_List;
}

Processor* scheduler::getRR_List()
{
	return RR_List;
}

Queue<Process*> scheduler::getBLK()
{
	return BLK;
}

Queue<Process*> scheduler::getTRM()
{
	return TRM;
}

Process* scheduler::getRUN()
{
	return RUN;
}

int scheduler::getFCFSCount()
{
	return 	FCFSCount;
}

int scheduler::getSJFCount()
{
	return SJFCount;
}

int scheduler::getRRCount()
{
	return RRCount;
}

int scheduler::getProcessorsCount()
{
	return ProcessorsCount;
}

scheduler::~scheduler()
{
	//for(int )
}
