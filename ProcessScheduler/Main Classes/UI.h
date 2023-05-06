#pragma once

#include "Processor.h"
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
	void PrintSilentMode(bool);

	//Takes the file name as input from the user
	string InputFileName(bool isError = 0);

	//Main function for printing info each Time step
	void PrintTimeStep(const Queue<Process*>&, const Queue<Process*>&, Processor**, int, int, int, int, int);
};	