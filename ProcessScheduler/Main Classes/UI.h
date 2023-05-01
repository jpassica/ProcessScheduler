#pragma once
#include "FCFS_Processor.h"
#include "SJF_Processor.h"
#include "RR_Processor.h"
#include <iostream>
#include <string>

using namespace std;

class UI
{
	UI_Mode CrntMode;

public:
	//Reads a line of input from the user
	string ReadLine();

	//Prints a string to the console
	void WriteLine(string);

	//Asks the user to choose from interface modes
	UI_Mode InputInterfaceMode();

	//Prints the needed info in silent mode
	void PrintSilentMode(bool StartorEnd);

	//Takes the file name as input from the user
	string InputFileName();

	//Main function for printing info each time step
	void TimeStepOut(const Queue<Process*>& BLK_List, const Queue<Process*>& TRM_List, 
		Processor** ProcessorsList, int NF, int NS, int NR, int timeStep);
};	