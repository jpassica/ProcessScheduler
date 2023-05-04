#pragma once
#include "List.h"
#include "..\Main Classes\Process.h"

class ProcessList : public List<Process*>
{
public:
	int SearchByID(int PID)
	{
		if (!headPtr)
			return 0;

		Node<Process*>* curPtr = headPtr;
		int position(1);					//position starts from 1

		while (curPtr)
		{
			if (curPtr->getItem()->GetID() == PID)
			{
				return position;
			}
			curPtr = curPtr->getNext();
			position++;
		}
		return 0;
	}

	bool DeleteByID(int PID, Process*& killedProcess)
	{
		int position = SearchByID(PID);

		if (!position)
			return false;

		killedProcess = getEntry(position);

		remove(position);

		return true;
	}
};
