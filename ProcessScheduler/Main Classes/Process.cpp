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
	totalIO_D = 0;
	ProcessedTime = 0;
	child = nullptr;
	firstTimeExecution = 1;
}

ostream& operator<<(ostream& out, const Process* P) 
{
	out << P->PID;
	return out;
}

//Set the response time as FCPU is the time when the process is ready to be processed at first time
void Process::SetResponseTime(int FCPU) 
{
	ResponseTime = FCPU - ArrivalTime;

	//since this function will only be used once for each process
	//which is when it is being executed for the first time,
	//the function changes this data member to false 
	//to stop the processors from overrwriting the RT

	firstTimeExecution = 0;
}

void Process::SetProcessedTime(int t)
{
	ProcessedTime = t;
}

void Process::AddIORequest(int IO_R, int IO_D)
{
	IO_Pairs* newRequest = new IO_Pairs(IO_R, IO_D);

	IO_PairsQ.Enqueue(newRequest);

	totalIO_D += IO_D;
}

int Process::GetCPUTime() const
{
	return CPUTime;
}

// Set the termination time as TT is the time when the process go to TRM list after processing 
void Process::SetTerminationTime(int TT) 
{
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

int Process::GetArrivalTime() const
{
	return ArrivalTime;
}

int Process::GetWaitingTime() const
{
	return WaitingTime;
}

int Process::GetTerminationTime() const
{
	return TerminationTime;
}

int Process::GetTotalIO_D() const
{
	return totalIO_D;
}

ProcessState Process::GetProcessState() const
{
	return CrntState;
}

Process* Process::GetChild() const
{
	return child;
}

int Process::GetProcessedTime() const
{
	return ProcessedTime;
}

int Process::GetRemainingCPUTime() const
{
	return CPUTime - ProcessedTime;
}

void Process::ChangeProcessState(ProcessState NewState)
{
	CrntState = NewState;
}

void Process::ExecuteProcess()
{
	ProcessedTime++;
}

bool Process::isFirstExecution() const
{
	return firstTimeExecution;
}

Process::~Process() {
}

void Process::Setchild(Process* c)
{
	child = c;
}
