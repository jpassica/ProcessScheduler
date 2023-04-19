#pragma once
#include "Processor.h"
#include "..\DS Classes\List.h"

class FCFS_Processor : public Processor
{
	List<Process*> ReadyList;

public:
	FCFS_Processor(int ID , Scheduler*);

	//Picks the next process to run according to 'first come first serve'
	virtual void ScheduleAlgo() override;

	//calculates and returns finish time according to CT of processes
	virtual int CalcFinishTime() override;

	//adds the passed process to the ReadyList
	virtual void AddToReadyQueue(Process* pReady) override;

	virtual bool isReadyQueueEmpty() const override;

	virtual bool fromReadyToRun(int crntTimeStep) override;

	virtual int GetRDYCount() const override;

	void RandomKill(int randomID);

	virtual void printRDY() const override;
};

