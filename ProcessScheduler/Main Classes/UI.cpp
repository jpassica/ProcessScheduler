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
			cout << "Processor " << ProcessorID << "[FCFS]" << ": " << ptr1->getRDY().getCount() << " : ";
			ptr1->getRDY().Print();
		}
		else if (ptr2) //Processor is RR
		{
			cout << "Processor " << ProcessorID << "[RR]" << ": " << ptr2->getRDY().getCount() << " : ";
			ptr2->getRDY().Print();
		}
		else if(ptr3) //Processor is SJF
		{
			cout << "Processor " << ProcessorID << "[SJF]" << ": " << ptr3->getRDY().getcount() << " : ";
			ptr3->getRDY().Print();
		}
		cout << endl;

	}
	cout << "--------------------   BLK Processes --------------------------------" << endl;
	Queue<Process*> BLK = pSch->getBLK();
	cout << BLK.getCount() << " BLK: ";
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
		Processor* currProcessor = pSch->getProcessors_List()[i];
		Process* running = currProcessor->getRunPtr();
		if (running)
			cout << running->GetPID() << "(P" << currProcessor->getID() << "),";
	}
	cout << endl;

	cout << "--------------------   TRM Processes --------------------------------" << endl;
	Queue<Process*> TRM = pSch->getTRM();
	cout << TRM.getCount() << " TRM: ";
	TRM.Print();
	cout << endl;

	cout << "Press Any Key To Move To Next Step\n";
	getchar();
}