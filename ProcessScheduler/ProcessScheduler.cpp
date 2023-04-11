// ProcessScheduler.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include"Queue.h"
#include"PriQqueue.h"
#include"PriorityQueue.h"
#include <iostream>
using namespace std;

int main()
{
    PriorityQueue<int> Q;
    
    Q.Enqueue(5, 3);
    Q.Enqueue(2 , 3);
    Q.Enqueue(4, 10);
    Q.Enqueue(1,0);    
    Q.Enqueue(8,5);
    Q.Print();
    cout << endl;
    int a , b;
    Q.Dequeue();
    Q.Print();
    cout << endl;

    Q.Dequeue();
    Q.Print();
    cout << endl;

    Q.Dequeue();
    Q.Print();
    cout << endl;

    Q.QueueFront(a, b);
    cout << a << " " << b;
    
   
}

