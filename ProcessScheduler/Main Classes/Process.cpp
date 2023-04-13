#include "Process.h"


//Process Constructor
Process::Process(int pid, int AT, int CT) :PID(pid), ArrivalTime(AT), CPUTime(CT) {}

//Initialization for (IO_R,IO_D) 2_d array
void Process::InitialIO(int n) {
	for (int i = 0; i < 2; i++)
	{
		IOPairs[i] = new int[n];
	}
}

//Set the values of IO_R, IO_D for the process
void Process::SetIO(int i, int IO_D, int IO_R) {
	IOPairs[0][i] = IO_D;
	IOPairs[1][i] = IO_R;
}
//Set the response time as FCPU is the time when the process is ready to be processed at first time
void Process::SetResponseTime(int FCPU) {
	ResponseTime = FCPU - ArrivalTime;
}

// Set the termination time as TT is the time when the process go to TRM list after processing 
void Process::SetTerminationTime(int TT) {
	TerminationTime = TT;
	TurnAroundTime = TerminationTime - ArrivalTime;
	WaitingTime = TurnAroundTime - CPUTime;
}

//Getter for Process ID
int Process::GetPID() const {
	return PID;
}

//Destructor for deallocating the dynamic array we used
Process::~Process() {
	delete[] IOPairs[0];
	delete[] IOPairs[1];
	delete[] IOPairs;
}