#include "UI.h"
using namespace std;

UI::UI(scheduler* s)
{
	pSch = s;
}

string UI::ReadLine()
{
	string s;
	getline(cin, s);
	return  s;
}

void UI::WriteLine(string s)
{
	cout << s;
}

void UI::TimeStepOut()
{

	cout << "Current Timestep: " << pSch->getTimeStep() << endl;
	cout << "--------------------   RDY Processes --------------------------------" << endl;
	for (int i = 0; i < pSch->getProcessorsCount(); i++)
	{

		int ProcessorID = pSch->getProcessors_List()[i]->getID();
		FCFS_Processor* ptr1 = dynamic_cast<FCFS_Processor*>(pSch->getProcessors_List()[i]);
		RR_Processor* ptr2 = dynamic_cast<RR_Processor*>(pSch->getProcessors_List()[i]);
		SJF_Processor* ptr3 = dynamic_cast<SJF_Processor*>(pSch->getProcessors_List()[i]);
		if (ptr1)
		{
			Queue<Process> RDY = ptr1->getRDY();
			cout << "Processor " << ProcessorID << "[FCFS]" << ": " << RDY.getCount() << " : ";
			RDY.Print();
		}
		else if (ptr2)
		{
			Queue<Process> RDY = ptr1->getRDY();
			cout << "Processor " << ProcessorID << "[RR]" << ": " << RDY.getCount() << " : ";
			RDY.Print();
		}
		else if(ptr3)
		{
			PriorityQueue<Process> RDY=ptr3->getRDY();
			cout << "Processor " << ProcessorID << "[SJF]" << ": " << RDY.getcount() << " : ";
			RDY.Print();
		}
		cout << endl;

	}
	cout << "--------------------   BLK Processes --------------------------------" << endl;
	Queue<Process> BLK = pSch->getBLK();
	cout << BLK.getCount() << ": ";
	BLK.Print();
	//To be continued

}