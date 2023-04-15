#include "DS Classes/PriorityQueue.h"
#include "DS Classes/Queue.h"
#include "DS Classes/List.h"
#include "Main Classes/scheduler.h"
#include "Main Classes/UI.h"
#include <iostream>
#include "Main Classes/Processor.h"
#include "Main Classes/FCFS_Processor.h"
using namespace std;

int main()
{
	/*PriorityQueue<int>Q;
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
	cout << endl;*/

	FCFS_Processor p1(1);
	

	/*List<int> L1;

	L1.insert(1, 2);
	L1.insert(2, 4);
	L1.insert(1, 1);
	L1.insert(4, 5);
	L1.insert(3, 3);
	L1.insert(6, 6);
	L1.insert(7, 7);

	L1.Print();


	L1.clear(); L1.Print();

	cout << L1.getLength() << endl;*/
	
	cout << endl;
	scheduler S;
	S.getBLK();

	cout << S.ReadInputFile("Sample Input File.txt");

}

