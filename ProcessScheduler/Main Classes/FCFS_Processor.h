#pragma once
#include "Processor.h"
#include "..\DS Classes\List.h"

class FCFS_Processor : public Processor
{
	List<Process*> FCFS_Ready;

public:
	FCFS_Processor(int ID , scheduler*);

	//Picks the next process to run according to 'first come first serve'
	virtual void ScheduleAlgo() override;

	//calculates and returns finish time according to CT of processes
	virtual int CalcFinishTime() override;


	List<Process*>& getRDY();
};

