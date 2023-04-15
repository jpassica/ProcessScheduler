#include "Process.h"

//Process Constructor
Process::Process(int pid, int AT, int CT) :PID(pid), ArrivalTime(AT), CPUTime(CT), CrntState(NEW) 
{
	//initializing all data members
	TerminationTime = 0;
	TurnAroundTime = 0;
	WaitingTime = 0;
	ResponseTime = 0;
}

//Initialization for (IO_R,IO_D) 2_d array
void Process::InitialIO(int n) {
	for (int i = 0; i < 2; i++)
	{
		IOPairs[i] = new int[n];
	}
}

//Set the values of IO_R, IO_D for the process
void Process::SetIO(int i, int IO_R, int IO_D) {
	IOPairs[0][i] = IO_R;
	IOPairs[1][i] = IO_D;
}

void Process::SetIO(int IO_N, int IO_R, int IO_D, int j)
{

}


//Set the response time as FCPU is the time when the process is ready to be processed at first time
void Process::SetResponseTime(int FCPU) {
	ResponseTime = FCPU - ArrivalTime;
}

int Process::GetCPUTime() const
{
	return CPUTime;
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

int Process::GetTurnAroundTime() const
{
	return TurnAroundTime;
}

int Process::GetResponseTime() const
{
	return ResponseTime;
}

int Process::GetWaitingTime() const
{
	return WaitingTime;
}

ProcessState Process::GetProcessState()
{
	return CrntState;
}

void Process::ChangeProcessState(ProcessState NewState)
{
	CrntState = NewState;
}

void Process::DecrementCPUTime()
{
	CPUTime--;
}

//Destructor for deallocating the dynamic array we used
Process::~Process() {
	delete[] IOPairs[0];
	delete[] IOPairs[1];
	delete[] IOPairs;
}

ostream& operator<<(ostream& out, const Process& P)
{
	out << P.PID;
	return out;
}
