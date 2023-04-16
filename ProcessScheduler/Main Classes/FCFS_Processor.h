#pragma once
#include "Processor.h"
#include "..\DS Classes\List.h"

class FCFS_Processor : public Processor
{
	List<Process*> ReadyList;

public:
	FCFS_Processor(int ID);

	//Picks the next process to run according to 'first come first serve'
	virtual void ScheduleAlgo() override;

	//calculates and returns finish time according to CT of processes
	virtual int CalcFinishTime() override;


	List<Process*> getRDY();
};

