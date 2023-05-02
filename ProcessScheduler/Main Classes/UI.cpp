#include "UI.h"
#include <Windows.h>
using namespace std;

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

UI_Mode UI::InputInterfaceMode()
{
	int ModeCode(0);

	cout << "\nChoose the interface mode for the simulation: \n"
		<< "1. Interactive Mode\n"
		<< "2. StepByStep Mode\n"
		<< "3. Silent Mode\n"
		<< ">";

	cin >> ModeCode;
	
	if (ModeCode == 1)
		CrntMode = Interactive;
	else if (ModeCode == 2)
		CrntMode = StepByStep;
	else
		CrntMode = Silent;

	return CrntMode;
}

void UI::PrintSilentMode(bool StartorEnd)
{
	//if StartorEnd = 0, this means we are at the beginning of the simulation
	
	if (!StartorEnd)
	{
		cout << "Silent Mode: Simulation started.... \n\n";
	}
	else		//if StartorEnd = 1, this means we are at the end
	{
		cout << "Simulation ended, output file created!\n\n";
	}
}

string UI::InputFileName()
{
	string FileName;

	cout << "Please enter input file name: >"; 
	cin >> FileName;

	return FileName;
}

void UI::TimeStepOut(const Queue<Process*>& BLK_List, const Queue<Process*>& TRM_List, 
	Processor** ProcessorsList, int NF, int NS, int NR, int NE, int timeStep)
{
	int ProcessorsCount = NF + NS + NR + NE;

	cout << "Current Time Step: " << timeStep << endl;  //printing current timestep
	cout << "--------------------   RDY Processes  -------------------------------" << endl;  
	for (int i = 0; i < ProcessorsCount; i++)
	{
		int ProcessorID = ProcessorsList[i]->getID();
		
		cout << "Processor " << ProcessorID;

		//printing the type of the processor 
		if (i >= 0 && i < NF)
			cout << " [FCFS]";
		else if (i >= NF && i < NF + NS)
			cout << " [SJF ]";
		else if (i >= NF + NS && i < NF + NS + NR)
			cout << " [RR  ]";
		else
			cout << " [EDF ]";

		cout << ": " << ProcessorsList[i]->GetRDYCount() << " RDY : ";
		ProcessorsList[i]->PrintRDY();
		cout << endl;
	}
	cout << "--------------------   BLK Processes  -------------------------------" << endl;

	cout << BLK_List.getCount() << " BLK: ";
	BLK_List.Print();
	cout << endl;

	cout << "--------------------   RUN Processes  -------------------------------" << endl;
	
	//calculating no. of running processes
	int runningCount = 0;
	for (int i = 0; i < ProcessorsCount; i++)
	{
		Process* running = ProcessorsList[i]->GetRunPtr();
		if (running)
			runningCount++;
	}

	cout << runningCount << " RUN: ";
	
	//printing running processes by getting the RunPtr in each processor
	bool found = false;		//used to detect first output to adjust format 
	for (int i = 0; i < ProcessorsCount; i++)
	{
		Process* running = ProcessorsList[i]->GetRunPtr();
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
	
	cout << endl << endl;
	

	if (CrntMode == Interactive)
	{
		cout << "Press Any Key To Move To Next Step\n";

		if (!timeStep)
			getchar();

		getchar();
	}
	else if (CrntMode == StepByStep)
		Sleep(1000);
	
}