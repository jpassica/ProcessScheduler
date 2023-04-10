#pragma once
#include<iostream>
#include"Node.h"
using namespace std;

template <class T>
class Queue
{
private:
	Node<T>* back; //Normal Queue implemented by the idea of circular queue

public:
	Queue()
	{
		back = nullptr;
	}
	bool isEmpty()
	{
		if (back)
			return false;
		else
			return true;
	}
	void Enqueue(T data)
	{
		Node<T>* N = new Node<T>;
		N->setItem(data);
		if (!isEmpty())
		{
			N->setNext(back->getNext());
			back->setNext(N);
		}
		else
			N->setNext(N);
		back = N;
	}
	bool Dequeue()
	{
		if (isEmpty())
			return false;
		Node<T>* Temp;
		Temp = back->getNext();
		back->setNext(Temp->getNext());
		if (back == Temp)
			back = nullptr;
		delete Temp;
		return true;
	}
	T Queue_front()
	{
		if (!isEmpty())
			return back->getNext()->getItem();
	}
	void Print()
	{
		if (!back)
			return;
		Node<T>* ptr = back->getNext();
		while (ptr != back)
		{
			cout << ptr->getItem();
			ptr = ptr->getNext();
		}
		cout << back->getItem();
		cout << endl;
	}
	~Queue()
	{
		while (back)
			Dequeue();
	}	
};



