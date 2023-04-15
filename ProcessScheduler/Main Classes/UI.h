#pragma once
#include "../DS Classes/PriorityQueue.h"
#include "../DS Classes/Queue.h"
//#include "Processor.h"
#include "FCFS_Processor.h"
#include "SJF_Processor.h"
#include "RR_Processor.h"
#include "scheduler.h"
#include "Process.h"
#include <iostream>
#include <string>

using namespace std;
class scheduler;

class UI
{
	scheduler* pSch;
public:
	UI(scheduler*);	
	string ReadLine();
	void WriteLine(string);
	void TimeStepOut();
	void EndOut();
};	