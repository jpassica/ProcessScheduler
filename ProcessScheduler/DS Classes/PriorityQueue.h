#pragma once
#define MAXSIZE 100000
#include"KeyNode.h"
#include<iostream>
using namespace std;
template <class T>
class PriorityQueue 
{
//min-Heap implementation
private:
	KeyNode<T>* arr[MAXSIZE];
	int size;

	bool shiftUpward(int ind)
	{
		//checking range
		if (ind >= size || ind < 0)
			return false;
		
		while (ind / 2 >= 0 && arr[ind / 2]->getKey() > arr[ind]->getKey())
		{
			swap(arr[ind / 2], arr[ind]);
			ind = ind / 2;
		}
		return true;
	}
	void MaxHeapify(int ind)
	{
		int l, r, min = ind;   //left , right pointers of binary 
		l = 2 * ind;
		r = l + 1;
		if (l >= size && r >= size)
			return;
		if (l < size && arr[l]->getKey() < arr[ind]->getKey())
			min = l;
		if (r < size && arr[r]->getKey() < arr[min]->getKey())
			min = r;
		if (min != ind)
		{
			swap(arr[ind], arr[min]);
			return MaxHeapify(min);
		}
	}

public:
	PriorityQueue()
	{
		size = 0;
		for (int i = 0; i < MAXSIZE; i++)
			arr[i] = nullptr;
	}

	PriorityQueue(const PriorityQueue<T>& Q) :PriorityQueue() //copy constructor
	{
		if (Q.size)
		{
			size = Q.size;
			for (int i = 0; i < size; i++)
			{
				KeyNode<T>* N = new KeyNode<T>();
				N->setItem(Q.arr[i]->getItem());
				N->setKey(Q.arr[i]->getKey());
				arr[i] = N;
			}
		}
	}
	void Enqueue(T data , int key)
	{
		KeyNode<T>* N=new KeyNode<T>();
		N->setItem(data);
		N->setKey(key);
		arr[size] = N;
		size++;
		shiftUpward(size - 1);
	}
	bool Dequeue()
	{
		if (!size)
			return false;

		arr[0] = arr[size - 1];
		size--;
		MaxHeapify(0);
		return true;
	}
	bool QueueFront(T& data , int & key)
	{
		if (!size)
			return false;

		data = arr[0]->getItem();
		key = arr[0]->getKey();
		return true;
	}
	void Print()
	{
		for (int i = 0; i < size; i++)
			cout << "( " << arr[i]->getItem()<< " , " << arr[i]->getKey() << " )";
	}
	int getcount()
	{
		return size;
	}
	~PriorityQueue(){}

};


