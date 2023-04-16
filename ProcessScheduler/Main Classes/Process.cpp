#include "Process.h"

//Process Constructor
Process::Process(int pid, int AT, int CT, int IO_N) 
	: PID(pid), ArrivalTime(AT), CPUTime(CT), IO_N(IO_N), CrntState(NEW)
{
	//initializing all data members
	TerminationTime = 0;
	TurnAroundTime = 0;
	WaitingTime = 0;
	ResponseTime = 0;
}

ostream& operator<<(ostream& out, const Process& P) 
{
	out << P.PID;
	return out;
}

//Set the response time as FCPU is the time when the process is ready to be processed at first time
void Process::SetResponseTime(int FCPU) {
	ResponseTime = FCPU - ArrivalTime;
}

void Process::AddIORequest(int IO_R, int IO_D)
{
	IO_Pairs* newRequest = new IO_Pairs(IO_R, IO_D);

	IO_PairsQ.Enqueue(newRequest);
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

ProcessState Process::GetProcessState() const
{
	return CrntState;
}

void Process::ChangeProcessState(ProcessState NewState)
{
	CrntState = NewState;
}

void Process::RunProcess()
{
	CPUTime--;
}

//Destructor for deallocating the dynamic array we used
Process::~Process() {
	/*delete[] IOPairs[0];
	delete[] IOPairs[1];
	delete[] IOPairs;*/
}