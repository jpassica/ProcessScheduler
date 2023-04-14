#pragma once
#include "Process.h"
#include<iostream>
using namespace std;
//#include 

enum ProcessorState { IDLE, BUSY };

class Processor
{
private:
	int ID;
	ProcessorState CrntState;
	int busyTime, idleTime;
	int finishTime;
	
protected:
	Process* RunPtr;								//Ptr to the executing process
	//Scheduler* pScheduler;

public:
	Processor(int ID);								//ctor receives ID from the schedule
	friend ostream& operator<<(ostream&, const Processor&);  //outputs Processor's ID
	virtual void ScheduleAlgo() = 0;
	double CalcPLoad(int TotalTRT);					//calculates Processor Load and re
	double CalcPUtil() const;
	int getID();
};