#include "UI.h"
#include <Windows.h>
using namespace std;

string UI::ReadLine()
{
	string Line;
	getline(cin, Line);
	return Line;
}

void UI::WriteLine(string Line)
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

void UI::PrintSilentMode(bool StartorEnd)
{
	//if StartorEnd = 0, this means we are at the beginning of the simulation
	
	if (!StartorEnd)
	{
		cout << "Silent Mode: Simulation started.... \n\n";
	}
	else		//if StartorEnd = 1, this means we are at the end
	{
		Sleep(500);
		cout << "Simulation ended, output file created!\n\n";
	}
}

string UI::InputFileName(bool isError)
{
	string FileName;

	if (isError)
		cerr << "Error! Are you sure there's a file with that name?\n Please enter a valid input file name: >";
	else
		cout << "Please enter input file name: > ";

	cin >> FileName;

	cout << endl;

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
	
	//calculating no. of RunningProcess processes
	int RunningCount = 0;
	for (int i = 0; i < ProcessorsCount; i++)
	{
		Process* RunningProcess = ProcessorsList[i]->GetRunPtr();
		if (RunningProcess)
			RunningCount++;
	}

	cout << RunningCount << " RUN: ";
	
	//printing RunningProcess processes by getting the RunPtr in each processor
	bool Found = false;		//used to detect first output to adjust format 
	for (int i = 0; i < ProcessorsCount; i++)
	{
		Process* RunningProcess = ProcessorsList[i]->GetRunPtr();
		if (RunningProcess)
		{
			if (Found)
				cout << ", ";
			cout << RunningProcess->GetPID() << "(P" << ProcessorsList[i]->getID() << ")";
			Found = true;
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
		Sleep(0.001);
	
}