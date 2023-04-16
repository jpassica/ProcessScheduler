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
	return s;
}

void UI::WriteLine(string s)
{
	cout << s << endl;
}

void UI::TimeStepOut()
{

	cout << "Current Timestep: " << pSch->getTimeStep() << endl;  //printing current timestep
	cout << "--------------------   RDY Processes --------------------------------" << endl;  
	for (int i = 0; i < pSch->getProcessorsCount(); i++)
	{
		int ProcessorID = pSch->getProcessors_List()[i]->getID(); 

		//dynamic casting is done to determine the type of processor to know call getRDY function. 
		//getRDY func. differs according to the type of processor & can't be done by polymorphisim.

		FCFS_Processor* ptr1 = dynamic_cast<FCFS_Processor*>(pSch->getProcessors_List()[i]);
		RR_Processor* ptr2 = dynamic_cast<RR_Processor*>(pSch->getProcessors_List()[i]);
		SJF_Processor* ptr3 = dynamic_cast<SJF_Processor*>(pSch->getProcessors_List()[i]);
		if (ptr1) //Processor is FCFC
		{
			List<Process*> RDY = ptr1->getRDY();
			cout << "Processor " << ProcessorID << "[FCFS]" << ": " << RDY.getCount() << " : ";
			RDY.Print();
		}
		else if (ptr2) //Processor is RR
		{
			List<Process*> RDY = ptr1->getRDY();
			cout << "Processor " << ProcessorID << "[RR]" << ": " << RDY.getCount() << " : ";
			RDY.Print();
		}
		else if(ptr3) //Processor is SJF
		{
			PriorityQueue<Process*> RDY = ptr3->getRDY();
			cout << "Processor " << ProcessorID << "[SJF]" << ": " << RDY.getcount() << " : ";
			RDY.Print();
		}
		cout << endl;

	}
	cout << "--------------------   BLK Processes --------------------------------" << endl;
	Queue<Process*> BLK = pSch->getBLK();
	cout << BLK.getCount() << ": ";
	BLK.Print();
	cout << endl;
	cout << "--------------------   RUN Processes --------------------------------" << endl;
	
	//calculating no. of running processes
	int runningCount = 0;
	for (int i = 0; i < pSch->getProcessorsCount(); i++)
	{
		Processor* currProcessor = pSch->getProcessors_List()[i];
		Process* running = currProcessor->getRunPtr();
		if (running)
			runningCount++;
	}

	cout << runningCount << " RUN: ";
	
	//printing running processes by getting the RunPtr in each processor
	for (int i = 0; i < pSch->getProcessorsCount(); i++)
	{
		if (i)
			cout << ", ";
		Processor* currProcessor = pSch->getProcessors_List()[i];
		Process* running = currProcessor->getRunPtr();
		if (running)
			cout << running->GetPID() << "(P" << currProcessor->getID() << ")";
	}
	cout << endl;

	cout << "--------------------   TRM Processes --------------------------------" << endl;
	Queue<Process*> TRM = pSch->getTRM();
	cout << TRM.getCount() << ": ";
	TRM.Print();
	cout << endl;

}