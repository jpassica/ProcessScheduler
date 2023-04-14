#pragma once
#include "Node.h"
#include <iostream>

template <class T>
class List					//Linked-based implementation of List ADT
{
	Node<T>* headPtr;
	int itemCount;

	Node<T>* getNodeAt(int position) const
	{
		if ((position >= 1) && (position <= itemCount))
		{
			Node<T>* curPtr = headPtr;
			for (int skip = 1; skip < position; skip++)
				curPtr = curPtr->getNext();

			return curPtr;
		}
		else
		{
			std::cerr << "Error! Empty list or invalid position.\n";
			return nullptr;
		}
	}

public:
	//default ctor
	List() : headPtr(nullptr), itemCount(0)	{}

	//copy ctor
	//List(const List<T>& aList)	{}

	bool isEmpty() const
	{
		return headPtr;
	}

	int getLength() const
	{
		Node<T>* curPtr = headPtr;
		int Count(0);

		while (curPtr)
		{
			curPtr = curPtr->getNext();
			Count++;
		}
		return Count;
	}

	bool insert(int newPosition, const T& newEntry)
	{
		bool ableToInsert = (newPosition >= 1) &&
			(newPosition <= itemCount + 1);
		if (ableToInsert)
		{
			// Create a new node containing the new entry
			Node<T>* newNodePtr = new Node<T>(newEntry);
			// Attach new node to chain
			if (newPosition == 1)
			{
				// Insert new node at beginning of chain
				newNodePtr->setNext(headPtr);
				headPtr = newNodePtr;
			}
			else
			{
				// Find node that will be before new node
				Node<T>* prevPtr = getNodeAt(newPosition - 1);
				// Insert new node after node to which prevPtr points
				newNodePtr->setNext(prevPtr->getNext());
				prevPtr->setNext(newNodePtr);
			} // end if
			itemCount++; // Increase count of entries
		} // end if
		return ableToInsert;
	}

	bool remove(int position)
	{
		bool ableToRemove = (position >= 1) && (position <= itemCount);
		if (ableToRemove)
		{
			Node<T>* curPtr = nullptr;
			if (position == 1)
			{
				// Remove the first node in the chain
				curPtr = headPtr; // Save pointer to node
				headPtr = headPtr->getNext();
			}
			else
			{
				// Find node that is before the one to delete
				Node<T>* prevPtr = getNodeAt(position - 1);
				// Point to node to delete
				curPtr = prevPtr->getNext();
				// Disconnect indicated node from chain by connecting the
				// prior node with the one after
				prevPtr->setNext(curPtr->getNext());
			}
			// Return node to system
			curPtr->setNext(nullptr);

			delete curPtr;
			curPtr = nullptr;
			itemCount--; // Decrease count of entries
		}
		return ableToRemove;
	}

	T getEntry(int position)
	{
		bool ableToGet = (position >= 1) && (position <= itemCount);
		if (ableToGet)
		{
			Node<T>* nodePtr = getNodeAt(position);
			return nodePtr->getItem();
		}
		else
		{
			std::cerr << "Error! Empty list or invalid position.\n";
		} 
	}

	void clear()
	{
		int baseCount(itemCount);			//I'm using this to avoid using a changing value (itemCount)		

		for (size_t i = 1; i < baseCount + 1; i++)
		{
			remove(1);
		}
	}

	void Print()
	{
		Node<T>* printPtr = headPtr;
		size_t i(1);

		while (printPtr && (i < itemCount + 1))
		{
			std::cout << printPtr->getItem() << "---";
			printPtr = printPtr->getNext();
		}
		std::cout << "NULL\n\n";
	}

	virtual ~List() {}
};