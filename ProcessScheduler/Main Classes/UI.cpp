#include "UI.h"
#include <Windows.h>
using namespace std;

string UI::ReadLine() const
{
	string Line;
	getline(cin, Line);
	return Line;
}

void UI::WriteLine(string Line) const
{
	cout << Line << endl;
}

UI_Mode UI::InputInterfaceMode()
{
	int ModeCode(0);

	cout << "\nChoose the interface mode for the simulation: \n"
		<< "1. Interactive Mode\n"
		<< "2. StepByStep Mode\n"
		<< "3. Silent Mode\n"
		<< "> ";

	cin >> ModeCode;

	cout << endl;
	
	if (ModeCode == 1)
		CrntMode = Interactive;
	else if (ModeCode == 2)
		CrntMode = StepByStep;
	else
		CrntMode = Silent;

	return CrntMode;
}

void UI::PrintSilentMode(bool StartorEnd) const
{
	//if StartorEnd = 0, this means we are at the beginning of the simulation
	
	if (!StartorEnd)
	{
		cout << "Silent Mode: Simulation started.... \n\n";
	}
	else		//if StartorEnd = 1, this means we are at the end
	{
		Sleep(350);
		cout << "Simulation ended, output file created!\n\n";
	}
}

string UI::ReadInputFileName(bool isError) const
{
	string FileName, FileCode;

	if (isError)
		cerr << "Error! Are you sure there's a file with that name?\n Please enter a valid input file name: >";
	else
		cout << "Please enter input file name: > ";

	cin >> FileCode;

	FileName = "TestCase" + FileCode;

	cout << endl;

	return FileName;
}

void UI::PrintTimeStep(const Queue<Process*>& BLK_List, const Queue<Process*>& TRM_List, 
	Processor** ProcessorsList, int NF, int NS, int NR, int NE, int timeStep) const
{
	int ProcessorsCount = NF + NS + NR + NE;

	cout << "Current Time Step: " << timeStep << endl;  //printing current timestep
	cout << "--------------------   RDY Processes  -------------------------------" << endl;  
	for (int i = 0; i < ProcessorsCount; i++)
	{
		int ProcessorID = ProcessorsList[i]->GetID();
		
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

		if (ProcessorsList[i]->IsStopped())
			cout << "OVERHEATED";

		ProcessorsList[i]->PrintRDY();
		cout << endl;
	}
	cout << "--------------------   BLK Processes  -------------------------------" << endl;

	cout << BLK_List.getCount() << " BLK: ";
	BLK_List.Print();
	cout << endl;

	cout << "--------------------   RUN Processes  -------------------------------" << endl;
	
	//Counting no. of running processes
	int RunningCount = 0;

	for (int i = 0; i < ProcessorsCount; i++)
	{
		if (ProcessorsList[i]->isExecutingProcess())
			RunningCount++;
	}

	cout << RunningCount << " RUN: ";
	
	//Printing RunningProcess processes by getting the RunPtr in each processor
	//Used to detect first output to adjust format 

	bool Found = 0;

	for (int i = 0; i < ProcessorsCount; i++)
	{
		if (ProcessorsList[i]->isExecutingProcess())
		{
			if (Found)
				cout << ", ";
			cout << ProcessorsList[i]->GetRunningProcessID() << "(P" << ProcessorsList[i]->GetID() << ")";
			Found = 1;
		}
	}
	cout << endl;

	cout << "--------------------   TRM Processes --------------------------------" << endl;

	cout << TRM_List.getCount() << " TRM: ";
	TRM_List.Print();
	
	cout << endl << endl;
	

	if (CrntMode == Interactive)
	{
		cout << "Press Enter Key To Move To Next Step\n";

		if (!timeStep)
			getchar();

		getchar();
	}
	else if (CrntMode == StepByStep)
		Sleep(0.000001);
	
}