#pragma once
#include"../DS Classes/PriorityQueue.h"
#include"../DS Classes/Queue.h"
#include"scheduler.h"
#include"Processor.h"
#include"Process.h"
#include<iostream>
#include<string>

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