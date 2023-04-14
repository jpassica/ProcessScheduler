#include "scheduler.h"

void scheduler::setProcessors(int NF, int NS, int NR , int timeslice)
{
	Processors_List = new Processor * [NF + NS + NR];
	RUN = new Process* [NF + NS + NR];
	for (int i = 0; i < NF; i++)
	{
		Processors_List[i] = new FCFS_Processor(i+1);
		RUN[i] = nullptr;
	}

	for (int i = NF; i < NS; i++)
	{
		Processors_List[i] = new SJF_Processor(i+NF+1);
		RUN[i] = nullptr;
	}

	for (int i = NS+NR; i < NR; i++)
	{
		Processors_List[i] = new RR_Processor(i+NF+NR+1 ,timeslice);
		RUN[i] = nullptr;
	}
}

scheduler::scheduler()	
{
	Timestep = 0;

	Processors_List = nullptr;

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

Processor** scheduler::getProcessors_List()
{
	return Processors_List;
}

const Queue<Process*>& scheduler::getBLK()
{
	return BLK;
}

const Queue<Process*>& scheduler::getTRM()
{
	return TRM;
}

Process** scheduler::getRUN()
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
