#include "UI.h"
using namespace std;

UI::UI(Scheduler* s)
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

void UI::TimeStepOut(const Queue<Process*>& BLK_List, const Queue<Process*>& TRM_List, 
	Processor** ProcessorsList, int NF, int NS, int NR, int timeStep)
{
	cout << "Current Time Step: " << timeStep << endl;  //printing current timestep
	cout << "--------------------   RDY Processes --------------------------------" << endl;  
	for (int i = 0; i < pSch->getProcessorsCount(); i++)
	{
		int ProcessorID = ProcessorsList[i]->getID();
		
		cout << "Processor " << ProcessorID;

		//printing the type of the processor 
		if (i >= 0 && i < NF)
			cout << " [FCFS]";
		else if (i >= NF && i < NF + NS)
			cout << " [SJF ]";
		else
			cout << " [RR  ]";

		cout << ": " << ProcessorsList[i]->GetRDYCount() << " RDY : ";
		ProcessorsList[i]->printRDY();
		cout << endl;
	}
	cout << "--------------------   BLK Processes --------------------------------" << endl;

	cout << BLK_List.getCount() << " BLK: ";
	BLK_List.Print();
	cout << endl;

	cout << "--------------------   RUN Processes --------------------------------" << endl;
	
	//calculating no. of running processes
	int runningCount = 0;
	for (int i = 0; i < pSch->getProcessorsCount(); i++)
	{
		Process* running = ProcessorsList[i]->getRunPtr();
		if (running)
			runningCount++;
	}

	cout << runningCount << " RUN: ";
	
	//printing running processes by getting the RunPtr in each processor
	bool found = false;		//used to detect first output to adjust format 
	for (int i = 0; i < pSch->getProcessorsCount(); i++)
	{
		Process* running = ProcessorsList[i]->getRunPtr();
		if (running)
		{
			if (found)
				cout << ", ";
			cout << running->GetPID() << "(P" << ProcessorsList[i]->getID() << ")";
			found = true;
		}
	}
	cout << endl;

	cout << "--------------------   TRM Processes --------------------------------" << endl;

	cout << TRM_List.getCount() << " TRM: ";
	TRM_List.Print();
	cout << endl;

	cout << "Press Any Key To Move To Next Step\n\n";
	getchar();
}