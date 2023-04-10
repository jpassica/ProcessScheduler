#pragma once
#include"KeyNode.h"
template <class T>
class PriQqueue 
{
private:
	KeyNode<T>* Front;
	KeyNode<T>* Back;
public:
	PriQqueue()
	{
		Front = nullptr;
		Back = nullptr;
	}
	bool isEmpty()
	{
		return (!Front?true:false);
		
	}
	void Enqueue(T data, int key)
	{
		KeyNode<T>* N = new KeyNode<T>();
		N->setItem(data);
		N->setKey(key);
		
		if (this->isEmpty()) //empty Queue
		{
			Back = N;
			Front = N;
			return;
		}


		if (key < Front->getKey())
		{
			N->setNext(Front);
			Front = N;
			return;
		}

		KeyNode<T>* ptr = Front;

		while (ptr->getNext() && ptr->getNext()->getKey() < key)
			ptr = ptr->getNext();

		N->setNext(ptr->getNext());
		if (ptr == Back)
			Back = N;
		ptr->setNext(N);

	}
	bool Dequeue(T& data)
	{
		if (isEmpty())
			return false;
		data = Front->getItem();
		KeyNode<T>* Temp=Front;
		Front = Front->getNext();
		delete Temp;
		Temp = nullptr;
		return true;
	}
	bool QueueFront(T& data ,int& k )
	{
		if (isEmpty())
			return false;
		data = Front->getItem();
		k = Front->getKey();
		return true;
	}
	void Print()
	{
		KeyNode<T>* ptr = Front;
		while (ptr)
		{
			cout << "("<<ptr->getItem()<<" , "<<ptr->getKey()<<") ";
			ptr = ptr->getNext();
		}
	}
	~PriQqueue()
	{
		KeyNode<T>* ptr = Front ;
		T temp;
		while (ptr)
		{
			Dequeue(temp);
		}
		Front = nullptr;
		Back = nullptr;
	}
};

