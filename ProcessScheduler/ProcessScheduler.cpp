#include "DS Classes/PriorityQueue.h"
#include"DS Classes/Queue.h"
#include <iostream>
#include "Main Classes/Processor.h"
#include "Main Classes/FCFS_Processor.h"
using namespace std;

int main()
{
	PriorityQueue<int>Q;
	Q.Enqueue(5, 3);
	Q.Enqueue(2, 3);
	Q.Enqueue(4, 10);
	Q.Enqueue(1, 0);
	Q.Enqueue(2, 4);
	Q.Print();
	cout << endl;

	Q.Dequeue();
	Q.Print();
	cout << endl;

	Q.Dequeue();
	Q.Print();
	cout << endl;

	Q.Dequeue();
	Q.Print();
	cout << endl;

	FCFS_Processor p1(1);
	
}

