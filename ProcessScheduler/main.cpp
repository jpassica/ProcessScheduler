#include "DS Classes/PriorityQueue.h"
#include "DS Classes/Queue.h"
#include "DS Classes/List.h"
#include "Main Classes/Scheduler.h"
#include "Main Classes/UI.h"
#include <iostream>
#include "Main Classes/Processor.h"
#include "Main Classes/FCFS_Processor.h"
using namespace std;

int main()
{
	Scheduler S;
	string fileName;
	cout << "Please enter input file name: "; cin >> fileName;
	S.Simulate(fileName);
}

