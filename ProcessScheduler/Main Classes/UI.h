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
	string ReadLine() const;

	//Prints a string to the console
	void WriteLine(string) const;

	//Asks the user to choose from interface modes
	UI_Mode InputInterfaceMode();

	//Prints the needed info in silent mode
	void PrintSilentMode(bool) const;

	//Takes the input file name as input from the user
	string ReadInputFileName(bool isError = 0) const;

	//Takes the output file name as input from the user
	string ReadOutputFileName() const;

	//Main function for printing info each Time step
	void PrintTimeStep(const Queue<Process*>&, const Queue<Process*>&, Processor**, int, int, int, int, int) const;
};	