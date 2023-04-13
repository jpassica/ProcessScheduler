#pragma once
#include"PriQqueue.h"
#include"Queue.h"
#include"scheduler.h"
#include"Processor.h"
#include"Process.h"
#include<iostream>
#include<string>

using namespace std;
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