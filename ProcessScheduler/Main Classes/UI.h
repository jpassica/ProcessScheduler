#pragma once
#include "../DS Classes/PriorityQueue.h"
#include "../DS Classes/Queue.h"
#include "FCFS_Processor.h"
#include "SJF_Processor.h"
#include "RR_Processor.h"
#include "Scheduler.h"
#include "Process.h"
#include <iostream>
#include <string>

using namespace std;
class Scheduler;

class UI
{
	Scheduler* pSch;
public:
	UI(Scheduler*);	
	string ReadLine();
	void WriteLine(string);
	void TimeStepOut(const Queue<Process*>& BLK_List, const Queue<Process*>& TRM_List, 
		Processor** ProcessorsList, int NF, int NS, int NR, int timeStep);
};	